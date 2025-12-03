#include "flecs.h"
#include "Organism.h"
#include "raylib.h"
#include "organism/data/Velocity.h"
#include "organism/organisms/mover/Mover.h"
#include "organism/organisms/eater/Eater.h"
#include "world/systems/Movement.h"
#include "world/systems/Death.h"
#include "world/systems/FlowerReproduction.h"
#include "world/systems/Eating.h"
#include "organism/percepts/Touch.h"
#include <cmath>
#include <random>
#include "world/plants/Flower.h"
#include "data/Status.h"
#include <vector>
#include <type_traits>
#include <utility>
#include <concepts>
#include <algorithm>
#include <thread>
#include "Entity.h"

/// <summary>
/// Draws all organisms and plants in the flecs world.
/// </summary>
/// <param name="world">The flecs world to draw from</param>
void draw(flecs::world& world);

/// <summary>
/// Spawns a number of organisms of the given type within the specified rectangular area,
/// adding them to the chunk grid for spatial partitioning.
/// </summary>
/// <typeparam name="Brain">The Brain type of the organism to spawn</typeparam>
/// <param name="organism">The organism factory to use for creating organisms</param>
/// <param name="count">Number of organisms to spawn</param>
/// <param name="minX">Minimum X coordinate of the spawn area</param>
/// <param name="minY">Minimum Y coordinate of the spawn area</param>
/// <param name="maxX">Maximum X coordinate of the spawn area</param>
/// <param name="maxY">Maximum Y coordinate of the spawn area</param>
/// <param name="chunkGrid">Reference to the chunk grid to place new organisms into</param>
/// <param name="chunkSize">Size of each chunk in the chunk grid</param>
/// <param name="chunkCols">Number of columns in the chunk grid</param>
template <typename Brain>
void spawnOrganisms(simbio::organism::Organism<Brain>& organism, int count, float minX, float minY, float maxX, float maxY, 
    std::vector<std::vector<simbio::organism::Entity>>& chunkGrid, int chunkSize, int chunkCols);

int main() {
    using namespace simbio::organism;

    flecs::world world;

    //world.set_threads(std::thread::hardware_concurrency() - 1);

    // Currently, displayWidth and displayHeight must be multiples of FLOWER_SPAWN_BOX_SIZE for flower spawning...
    // ...and a multiple of 16 for chunks.
    constexpr float FLOWER_SPAWN_BOX_SIZE = 256;
    const int DISPLAY_WIDTH = 1024;
    const int DISPLAY_HEIGHT = 1024;
    const float TIME_STEP = 0.1f;
    const int SIM_SPEED = 100;
    const int CHUNK_SIZE = 16;
    const int CHUNK_COLS = DISPLAY_WIDTH / CHUNK_SIZE;
    const int CHUNK_ROWS = DISPLAY_HEIGHT / CHUNK_SIZE;

    // Create chunk grid for spatial partitioning.
    std::vector<std::vector<Entity>> chunkGrid(CHUNK_COLS * CHUNK_ROWS);

	// Enforce Legs size constraints.
	Organs::registerOrganObservers(world);

    // Spawn some Mover entities.
    mover::Mover mover(world);
    spawnOrganisms(mover, 3, 100.0f, 100.0f, 200.0f, 200.0f, chunkGrid, CHUNK_SIZE, CHUNK_COLS);

    // Spawn some Eater entities.
    eater::Eater eater(world);
    spawnOrganisms(eater, 3, 800.0f, 800.0f, 900.0f, 900.0f, chunkGrid, CHUNK_SIZE, CHUNK_COLS);;

    // Register MoveIntent and Movement systems.
    simbio::systems::Movement movement;
    movement.registerMoveIntentSystem(world);
    movement.registerMovementSystem(world, DISPLAY_WIDTH, DISPLAY_HEIGHT, TIME_STEP, chunkGrid, CHUNK_SIZE, CHUNK_COLS);

    // Register flower reproduction system.
    simbio::systems::FlowerReproduction flowerReproduction;
    flowerReproduction.registerFlowerReproductionSystem(world, DISPLAY_WIDTH, DISPLAY_HEIGHT, 
        TIME_STEP, chunkGrid, CHUNK_SIZE, CHUNK_COLS, CHUNK_ROWS);

    // Register eating / grabbing systems.
    simbio::systems::Eating eating;
    eating.registerGrabbingSystem(world, chunkGrid, CHUNK_SIZE, CHUNK_COLS, CHUNK_ROWS);
    eating.registerBitingSystem(world, chunkGrid, CHUNK_SIZE, CHUNK_COLS, CHUNK_ROWS);

    //Register Death system.
    auto* death = new simbio::systems::Death();
    death->registerDeathSystem(world, chunkGrid);
    death->registerFlowerDeathSystem(world, chunkGrid);

    // Spawn Flowers across the grid.
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> sizeDist(Flower::MIN_SIZE, Flower::MAX_SIZE);
    std::uniform_real_distribution<float> unitDist(0.0f, 1.0f);
    std::exponential_distribution<float> reproductionTimerDist(1.0f / Flower::MIN_REPRODUCTION_TIMER);
    

    // Making sure the flowers don't overlap
    for (float x = 0.0f; x < DISPLAY_WIDTH; x += FLOWER_SPAWN_BOX_SIZE) {
        for (float y = 0.0f; y < DISPLAY_HEIGHT; y += FLOWER_SPAWN_BOX_SIZE) {
            int spawned = 0;
            int attempts = 0;
            const int MAX_ATTEMPTS = 50; // avoid infinite loops if area gets crowded

            while (spawned < 5 && attempts < MAX_ATTEMPTS) {
                ++attempts;

                float spawnX = x + unitDist(rng) * FLOWER_SPAWN_BOX_SIZE;
                float spawnY = y + unitDist(rng) * FLOWER_SPAWN_BOX_SIZE;

                int chunkX = (std::min)((int)(spawnX / CHUNK_SIZE), CHUNK_COLS - 1);
                int chunkY = (std::min)((int)(spawnY / CHUNK_SIZE), CHUNK_ROWS - 1);
                int chunk = chunkY * CHUNK_COLS + chunkX;

                int size = sizeDist(rng);
                float radius = size * 0.5f;

                bool overlaps = false;

                // Check this chunk and its 8 neighbors for overlapping flowers
                for (int dy = -1; dy <= 1 && !overlaps; ++dy) {
                    int adjChunkY = chunkY + dy;
                    if (adjChunkY < 0) adjChunkY = CHUNK_ROWS - 1;
                    if (adjChunkY >= CHUNK_ROWS) adjChunkY = 0;

                    for (int dx = -1; dx <= 1 && !overlaps; ++dx) {
                        int adjChunkX = chunkX + dx;
                        if (adjChunkX < 0) adjChunkX = CHUNK_COLS - 1;
                        if (adjChunkX >= CHUNK_COLS) adjChunkX = 0;

                        auto& bucket = chunkGrid[adjChunkY * CHUNK_COLS + adjChunkX];
                        for (const auto& entry : bucket) {
                            // Only avoid overlapping other flowers; organisms can overlap plants
                            flecs::entity adjEntity(world, entry.flecsID);
                            if (!adjEntity.is_alive()) continue;
                            if (!adjEntity.has<Flower>()) continue;

                            float adjRadius = entry.getSize() * 0.5f;
                            float dxPos = entry.location.x - spawnX;
                            float dyPos = entry.location.y - spawnY;
                            float minDist = radius + adjRadius + 2.0f; // small gap

                            if (dxPos * dxPos + dyPos * dyPos <= minDist * minDist) {
                                overlaps = true;
                                break;
                            }
                        }
                    }
                }

                if (overlaps) {
                    // Try again with a different random point
                    continue;
                }

                // No overlap -> commit the flower
                Location location{ spawnX, spawnY, 0.0f, chunk };

                flecs::entity flower = world.entity()
                    .set<Location>(location)
                    .set<simbio::organism::Color>(Flower::FLOWER_COLOR)
                    .set<Flower>(Flower{ (float)size });

                int reproductionTime = (int)std::ceil((Flower::REPRODUCTION_TIMER_SPREAD + reproductionTimerDist(rng)) / TIME_STEP);
                using namespace simbio::systems;
                Entity e{ .flecsID = flower.id(), .color = Flower::FLOWER_COLOR, .location = location, 
                    .organs = Organs{ .flower = (float)size } };
                FlowerReproduction::reproductionQueue
                    [(FlowerReproduction::currentReproductionTick + reproductionTime) % Flower::MAX_REPRODUCTION_TICKS].push_back(e);

                chunkGrid[chunk].push_back(e);
                ++spawned;
            }
        }
    }


    InitWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, "SimBio");
    //SetTargetFPS(30);

    while (!WindowShouldClose()) {
        if (GetKeyPressed() != 0) break;

        BeginDrawing();
        ClearBackground(::Color{0, 0, 0, 255});

        for (int i = 0; i < SIM_SPEED; ++i) world.progress(TIME_STEP);
        printf("FPS: %d\n", GetFPS());

        draw(world);

        EndDrawing();
    }

    delete death;
    return 0;
}

void draw(flecs::world& world) {
        using namespace simbio::organism;

        // Initialize queries for displaying organisms and plants.
        static auto drawBodyQuery = world.query<const simbio::organism::Color, const Body, const Location>();
        static auto drawLegsQuery = world.query<const simbio::organism::Color, const Body, const Legs, const Location>();
        static auto drawMouthQuery = world.query<const simbio::organism::Color, const Body, const Mouth, const Location>();
        static auto drawFlowersQuery = world.query<const simbio::organism::Color, const Flower, const Location>();

        drawBodyQuery.each([](const simbio::organism::Color& color, const Body& body, const Location& location) {
            DrawCircleV({ location.x, location.y }, body.size * 0.5f, 
                ::Color{ color.r, color.g, color.b, 255 });
        });

        drawLegsQuery.each([](const simbio::organism::Color& color, const Body& body, const Legs& legs, const Location& location) {
            float radius = body.size * 0.5f;
            const int numLegs = 4;
            float legWidth = legs.size / 6.0f;


            const float offsets[numLegs] = {
                -2.0f * PI / 3.0f,
                -PI / 3.0f,
                PI / 3.0f,
                2.0f * PI / 3.0f
            };

            for (int i = 0; i < numLegs; ++i) {
                float angle = location.yaw + offsets[i];

                Vector2 start = {
                    location.x + std::cos(angle) * radius,
                    location.y + std::sin(angle) * radius
                };

                Vector2 end = {
                    location.x + std::cos(angle) * (radius + legs.size),
                    location.y + std::sin(angle) * (radius + legs.size)
                };

                DrawLineEx(start, end, legWidth, ::Color{ color.r, color.g, color.b, 255 });
            }
        });


        drawMouthQuery.each([](const simbio::organism::Color& color, const Body& body, const Mouth& mouth, const Location& location) {
            float radius = body.size * 0.5f;

            float a = location.yaw;
            Vector2 forward = { std::cos(a), std::sin(a) };

            Vector2 base = {
                location.x + forward.x * radius,
                location.y + forward.y * radius
            };

            float pincerLength = radius * 0.4f;
            float spread = 2.0f;

            float a1 = a + spread;
            float a2 = a - spread;

            Vector2 tip1 = {
                base.x + std::cos(a1) * pincerLength,
                base.y + std::sin(a1) * pincerLength
            };
            Vector2 tip2 = {
                base.x + std::cos(a2) * pincerLength,
                base.y + std::sin(a2) * pincerLength
            };

            DrawLineEx(base, tip1, 4.0f, ::Color{ 255, 255, 255, 255 });
            DrawLineEx(base, tip2, 4.0f, ::Color{ 255, 255, 255, 255 });
        });

        drawFlowersQuery.each([](const simbio::organism::Color& color, const Flower& flower, const Location& location) {
            DrawCircleV({ location.x, location.y }, flower.size * 0.5f, ::Color{ color.r, color.g, color.b, 255 });
        });
}

template <typename Brain>
	void spawnOrganisms(simbio::organism::Organism<Brain>& organism, int count, float minX, float minY, float maxX, float maxY, 
    std::vector<std::vector<simbio::organism::Entity>>& chunkGrid, int chunkSize, int chunkCols) {
    using namespace simbio::organism;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> xDist(minX, maxX);
    std::uniform_real_distribution<float> yDist(minY, maxY);
    int chunkRows = chunkGrid.size() / chunkCols;

    for (int i = 0; i < count; ++i) {
        float x = xDist(rng);
        float y = yDist(rng);
        int chunkX = (std::min)((int)(x / chunkSize), chunkCols - 1);
        int chunkY = (std::min)((int)(y / chunkSize), chunkRows - 1);
        int chunk = chunkY * chunkCols + chunkX;

        Location location{ x, y, 0.0f, chunk };
        
        flecs::entity e = organism.create();
        e.set<Location>(location);
        e.set<Velocity>({ 0.0f, 0.0f });
        e.set<Status>({ 100.0f, 100.0f });

        Organs organs;
        if (e.has<Arms>()) organs.arms = e.get<Arms>();
        if (e.has<Body>()) organs.body = e.get<Body>();
        if (e.has<Ears>()) organs.ears = e.get<Ears>();
        if (e.has<Eyes>()) organs.eyes = e.get<Eyes>();
        if (e.has<Legs>()) organs.legs = e.get<Legs>();
        if (e.has<Mouth>()) organs.mouth = e.get<Mouth>();

        chunkGrid[chunk].emplace_back(
            e.id(), 
            e.has<simbio::organism::Color>() ? e.get<simbio::organism::Color>() : simbio::organism::Color{ 255, 255, 255 },
            e.get<Location>(),
            e.get<Status>(),
            e.get<Velocity>(),
            organs
        );
    }
}
