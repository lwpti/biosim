#include "flecs.h"
#include "raylib.h"
#include "world/World.h"
#include "systems/Systems.h"
#include "organisms/mover/Mover.h"
#include "organisms/eater/Eater.h"
#include "organs/OrganObserver.h"
#include <random>
#include <map>

/// <summary>
/// Draws all organisms and plants in the flecs world.
/// </summary>
void draw(flecs::world& world);

/// <summary>
/// Spawns a number of organisms of the given type within the specified rectangular area,
/// adding them to the chunk grid for spatial partitioning.
/// </summary>
template <typename Brain>
void spawnOrganismsInBox(biosim::organism::Organism<Brain>& organism, int count, 
    biosim::World& world, float minX, float minY, float maxX, float maxY);

// I recommend not changing this
static constexpr int FPS = 30;
// Do not change this
static constexpr float TIME_STEP = 0.1f;
// You can change this to speed up the sim!
static constexpr int SIM_SPEED = 1;

static Texture2D flowerTexture;

int main() {
    using namespace biosim;
    using namespace organism;
    
    biosim::World world{ TIME_STEP };
    // Sync simulation speed with real time using FPS
    world.simulationSpeed = std::max(1, (int)(SIM_SPEED / TIME_STEP / FPS));

    // Register Death system.
    systems::Death::registerDeathSystem(world);
    systems::Death::registerFlowerDeathSystem(world);

    // Register Hearing system.
    systems::Hearing::registerHearingSystem(world);

    // Register Vision system.
    systems::Vision::registerVisionSystem(world);

    // Enforce Legs size constraints.
    OrganObserver::registerOrganObservers(world);

    // Register MoveIntent and Movement systems.
    systems::Movement::registerMoveIntentSystem(world);
    systems::Movement::registerMovementSystem(world);

    //Register Biting system
    systems::Biting bitingSystem;
    bitingSystem.registerBitingSystem(world);

    // Register flower reproduction system.
    systems::FlowerReproduction::registerFlowerReproductionSystem(world);

    // Spawn Flowers before death & reproduction systems register to set initial timers
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> sizeDist(Flower::MIN_SIZE, Flower::MAX_SIZE);
    std::uniform_real_distribution<float> xDist(0.0f, World::WIDTH - 0.001f);
    std::uniform_real_distribution<float> yDist(0.0f, World::HEIGHT - 0.001f);
    for (int i = 0; i < 5000; ++i) 
        while (!world.spawnFlower((float)sizeDist(rng), xDist(rng), yDist(rng)));

    // Let the flower population stabilize
    for (int i = 0; i < 100000 / SIM_SPEED; i++) world.progress();

    // Spawn some Mover entities.
    mover::Mover mover(world.flecsWorld);
    spawnOrganismsInBox(mover, 10, world, 0.0f, 0.0f, 128.0f, 128.0f);

    // Spawn some Eater entities.
    eater::Eater eater(world.flecsWorld);
    spawnOrganismsInBox(eater, 10, world, 256.0f, 256.0f, 384.0f, 384.0f);

    // Register eating / grabbing systems.
    //systems::Eating eating;
    //eating.registerGrabbingSystem(flecsWorld, chunkGrid, CHUNK_SIZE, chunkCols, chunkRows);
    //eating.registerBitingSystem(flecsWorld, chunkGrid, CHUNK_SIZE, chunkCols, chunkRows);

    InitWindow(World::WIDTH, World::HEIGHT, "BioSim");
    SetTargetFPS(FPS);

    // Prep flower texture
    float maxRadius = Flower::MAX_SIZE / 2.0f;
    Image image = GenImageColor(Flower::MAX_SIZE, Flower::MAX_SIZE, ::BLANK);

    Vector2 center = { maxRadius, maxRadius };
    ImageDrawCircleV(&image, center, maxRadius, 
        ::Color(Flower::FLOWER_COLOR.r, Flower::FLOWER_COLOR.g, Flower::FLOWER_COLOR.b, 255));

    flowerTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    // Pause display to observe initial spawn; press any key to proceed
    while (!WindowShouldClose()) {
        if (GetKeyPressed() != 0) break;

        BeginDrawing();
        ClearBackground(::BLACK);

        draw(world.flecsWorld);

        EndDrawing();
    }

    SetExitKey(KEY_ENTER);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_RIGHT)) {
            world.simulationSpeed = world.simulationSpeed * 2;
        } else if (IsKeyPressed(KEY_LEFT)) {
            world.simulationSpeed = std::max(1, world.simulationSpeed / 2);
        }
        BeginDrawing();
        ClearBackground(::BLACK);

        static std::map<flecs::entity, Location> locations;
        // Special trick if sim speed is 1 (which means only 10 sim ticks per second)
        // Store old locations for use in next query
        if (world.simulationSpeed == 1) {
            static auto query = world.flecsWorld.query<Location, Body>();
            query.each([&](flecs::entity entity, Location& location, const Body& body) {
                locations[entity] = location;
            });
        }

        world.progress();
        if (GetFPS() < FPS) printf("FPS drop detected.\n");

        // Special trick if sim speed is 1 (which means only 10 sim ticks per second)
        // Draw 2 extra frames in that time with lerped movement to maintain 30 FPS
        if (world.simulationSpeed == 1) {
            static auto query = world.flecsWorld.query<Location, Body>();
            std::map<flecs::entity, Location> finalLocations;
            query.each([&](flecs::entity entity, Location& location, const Body& body) {
                finalLocations[entity] = location;
                location = Location::lerp(locations[entity], location, 0.33f);
            });
            draw(world.flecsWorld);
            EndDrawing();
            if (IsKeyPressed(KEY_RIGHT)) {
                world.simulationSpeed = std::min(world.simulationSpeed * 3, SIM_SPEED);
            } else if (IsKeyPressed(KEY_LEFT)) {
                world.simulationSpeed = std::max(world.simulationSpeed / 3, 1);
            }
            if (WindowShouldClose()) break;
            BeginDrawing();
            ClearBackground(::BLACK);
            query.each([&](flecs::entity entity, Location& location, const Body& body) {
                location = Location::lerp(locations[entity], finalLocations[entity], 0.67f);
            });
            draw(world.flecsWorld);
            EndDrawing();
            if (IsKeyPressed(KEY_RIGHT)) {
                world.simulationSpeed = std::min(world.simulationSpeed * 3, SIM_SPEED);
            } else if (IsKeyPressed(KEY_LEFT)) {
                world.simulationSpeed = std::max(world.simulationSpeed / 3, 1);
            }
            if (WindowShouldClose()) break;
            BeginDrawing();
            ClearBackground(::BLACK);
            query.each([&](flecs::entity entity, Location& location, const Body& body) {
                location = finalLocations[entity];
            });
        }

        draw(world.flecsWorld);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

template <typename Brain>
void spawnOrganismsInBox(biosim::organism::Organism<Brain>& organism, int count, 
    biosim::World& world, float minX, float minY, float maxX, float maxY) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> xDist(minX, maxX);
    std::uniform_real_distribution<float> yDist(minY, maxY);
    for (int i = 0; i < count; ++i) 
    while (!world.spawnOrganism<Brain>(organism, xDist(rng), yDist(rng)));
}

void draw(flecs::world& world) {
    using namespace biosim::organism;

    // Initialize queries for displaying organisms and plants.
    static auto drawBodyQuery = world.query<const biosim::organism::Color, 
        const Body, const Location>();
    static auto drawLegsQuery = world.query<const biosim::organism::Color, 
        const Body, const Legs, const Location>();
    static auto drawMouthQuery = world.query<const biosim::organism::Color, 
        const Body, const Mouth, const Location>();
    static auto drawFlowersQuery = world.query<const biosim::organism::Color, 
        const Flower, const Location>();

    drawBodyQuery.each([](const biosim::organism::Color& color, const Body& body, 
        const Location& location) {
        DrawCircleV({ location.x, location.y }, body.size * 0.5f, 
            ::Color{ color.r, color.g, color.b, 255 });
    });

    drawLegsQuery.each([](const biosim::organism::Color& color, const Body& body, 
        const Legs& legs, const Location& location) {
        float radius = body.size * 0.5f;
        const int numLegs = 4;

        const float offsets[numLegs] = { -2.0f * PI / 3.0f, -PI / 3.0f, 
            PI / 3.0f, 2.0f * PI / 3.0f };

        for (int i = 0; i < numLegs; ++i) {
            float angle = location.yaw + offsets[i];

            Vector2 start = { location.x + std::cos(angle) * radius, 
                location.y + std::sin(angle) * radius };

            Vector2 end = { location.x + std::cos(angle) * (radius + legs.size), 
                location.y + std::sin(angle) * (radius + legs.size) };

            DrawLineEx(start, end, 1.0f, ::Color{ color.r, color.g, color.b, 255 });
        }
    });

    drawMouthQuery.each([](const biosim::organism::Color& color, const Body& body, 
        const Mouth& mouth, const Location& location) {
        float radius = body.size * 0.5f;

        Vector2 forward = { std::cos(location.yaw), std::sin(location.yaw) };

        Vector2 base = { location.x + forward.x * radius, location.y + forward.y * radius  };

        float spread = 0.4f;

        float leftAngle = location.yaw + spread;
        float rightAngle = location.yaw - spread;

        Vector2 leftTip = { base.x + std::cos(leftAngle) * mouth.size, 
            base.y + std::sin(leftAngle) * mouth.size };
        Vector2 rightTip = { base.x + std::cos(rightAngle) * mouth.size, 
            base.y + std::sin(rightAngle) * mouth.size };

        DrawLineEx(base, leftTip, 1.0f, ::Color{ 255, 255, 255, 255 });
        DrawLineEx(base, rightTip, 1.0f, ::Color{ 255, 255, 255, 255 });
    });

    drawFlowersQuery.each([](const biosim::organism::Color& color, 
        const Flower& flower, const Location& location) {
        float radius = flower.size / 2.0f;
        float scale = flower.size / Flower::MAX_SIZE;

        Rectangle source { 0.0f, 0.0f, (float)flowerTexture.width, (float)flowerTexture.height };
        Rectangle dest { location.x, location.y, 
            flowerTexture.width * scale, flowerTexture.height * scale };
        Vector2 origin = { dest.width * 0.5f, dest.height * 0.5f };

        DrawTexturePro(flowerTexture, source, dest, origin, 0.0f, ::WHITE);
    });
}
