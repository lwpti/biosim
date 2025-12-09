#include "flecs.h"
#include "raylib.h"
#include "world/World.h"
#include "systems/Systems.h"
#include "organisms/mover/Mover.h"
#include "organisms/eater/Eater.h"
#include "organs/OrganObserver.h"
#include <random>

/// <summary>
/// Draws all organisms and plants in the flecs world.
/// </summary>
void draw(flecs::world& world);

/// <summary>
/// Spawns a number of organisms of the given type within the specified rectangular area,
/// adding them to the chunk grid for spatial partitioning.
/// </summary>
template <typename Brain>
void spawnOrganismsInBox(simbio::organism::Organism<Brain>& organism, int count, 
    simbio::World& world, float minX, float minY, float maxX, float maxY);

int main() {
    using namespace simbio;
    using namespace organism;
    
    simbio::World world{ 1024, 1024, 0.1f };
    world.simulationSpeed = 100;

    // Enforce Legs size constraints.
    OrganObserver::registerOrganObservers(world);

    // Register MoveIntent and Movement systems.
    systems::Movement::registerMoveIntentSystem(world);
    systems::Movement::registerMovementSystem(world);

    // Register flower reproduction system.
    systems::FlowerReproduction::registerFlowerReproductionSystem(world);

    // Register eating / grabbing systems.
    //systems::Eating eating;
    //eating.registerGrabbingSystem(flecsWorld, chunkGrid, CHUNK_SIZE, chunkCols, chunkRows);
    //eating.registerBitingSystem(flecsWorld, chunkGrid, CHUNK_SIZE, chunkCols, chunkRows);

    //Register Biting system
    systems::Biting bitingSystem;
    bitingSystem.registerBitingSystem(world);

    //Register Death system.
    systems::Death::registerDeathSystem(world);
    systems::Death::registerFlowerDeathSystem(world);

    //Register Hearing system.
    systems::Hearing::registerHearingSystem(world);

    //Register Vision system.
    systems::Vision::registerVisionSystem(world);

    // Spawn some Mover entities.
    mover::Mover mover(world.flecsWorld);
    spawnOrganismsInBox(mover, 3, world, 100.0f, 100.0f, 200.0f, 200.0f);

    // Spawn some Eater entities.
    eater::Eater eater(world.flecsWorld);
    spawnOrganismsInBox(eater, 3, world, 800.0f, 800.0f, 900.0f, 900.0f);;

    // Spawn Flowers across the grid.
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> sizeDist(Flower::MIN_SIZE, Flower::MAX_SIZE);
    std::uniform_real_distribution<float> xDist(0.0f, world.width - 0.001f);
    std::uniform_real_distribution<float> yDist(0.0f, world.height - 0.001f);
    for (int i = 0; i < 1000; ++i) world.spawnFlower((float)sizeDist(rng), xDist(rng), yDist(rng));

    InitWindow(world.width, world.height, "SimBio");
    SetTargetFPS(30);
    SetTargetFPS(30);

    // Pause display to observe initial spawn; press any key to proceed
    while (!WindowShouldClose()) {
        if (GetKeyPressed() != 0) break;

        BeginDrawing();
        ClearBackground(::Color{0, 0, 0, 255});

        //printf("FPS: %d\n", GetFPS());

        draw(world.flecsWorld);

        EndDrawing();
    }

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ENTER)) break;

        BeginDrawing();
        ClearBackground(::Color{0, 0, 0, 255});

        world.progress();
        //printf("FPS: %d\n", GetFPS());

        draw(world.flecsWorld);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

template <typename Brain>
void spawnOrganismsInBox(simbio::organism::Organism<Brain>& organism, int count, 
    simbio::World& world, float minX, float minY, float maxX, float maxY) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> xDist(minX, maxX);
    std::uniform_real_distribution<float> yDist(minY, maxY);
    for (int i = 0; i < count; ++i) world.spawnOrganism<Brain>(organism, xDist(rng), yDist(rng));
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
