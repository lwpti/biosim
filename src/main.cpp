#include "flecs.h"
#include "Organism.h"
#include "raylib.h"
#include "organism/data/Velocity.h"
#include "organism/organisms/mover/Mover.h"
#include "organism/organisms/eater/Eater.h"
#include "world/systems/Movement.h"
#include "world/systems/Death.h"
#include "world/systems/FlowerReproduction.h"
#include <cmath>
#include <random>
#include "data/Status.h"
#include "plants/Flower.h"

int main() {
    using namespace simbio::organism;
    using namespace simbio::plants;

    flecs::world world;

    const int displayWidth = 1000;
    const int displayHeight = 1000;
    const float timeStep = 0.1f;
    const int simSpeed = 10;

	// Enforce Legs size constraints
	Legs::registerLegsObserver(world);

    // Spawn some Mover entities
    mover::Mover mover(world);
    mover.create().set<Location>({ 100, 100, 0 }).set<Velocity>({ 0, 0 }).set<Status>({ 100, 100});
    mover.create().set<Location>({ 200, 200, 0 }).set<Velocity>({ 0, 0 }).set<Status>({ 100, 100 });
    mover.create().set<Location>({ 300, 300, 0 }).set<Velocity>({ 0, 0 }).set<Status>({ 100, 100 });

    // Spawn some Eater entities
    eater::Eater eater(world);
    eater.create().set<Location>({ 100, 100, 1 }).set<Velocity>({ 0, 0 }).set<Status>({ 100, 100 });
    eater.create().set<Location>({ 200, 200, 3 }).set<Velocity>({ 0, 0 }).set<Status>({ 100, 100 });
    eater.create().set<Location>({ 300, 300, 6 }).set<Velocity>({ 0, 0 }).set<Status>({ 100, 100 });

    // Spawn Flowers across the grid (~1 per 10x10 square)
    Color flowerColor{ 0, 228, 48, 255 };
    std::random_device rd;
    std::mt19937 rng(rd());
    std::exponential_distribution<float> reproductionTimerDist(1.0f / 15.0f);
    std::uniform_int_distribution<int> sizeDist(Flower::MIN_SIZE, Flower::MAX_SIZE);
    std::uniform_int_distribution<int> locationDist(0, 1000);

    for (float x = 5; x < displayWidth; x += 10) {
        for (float y = 5; y < displayHeight; y += 10) {
            world.entity()
                .set<Flower>(Flower{ sizeDist(rng), flowerColor, (int)std::ceil(15.0f + reproductionTimerDist(rng) / timeStep) })
                .set<Location>({ x, y, 0.0f });
        }
    }

    // Register MoveIntent and Movement systems
    simbio::systems::Movement movement;
    movement.registerMoveIntentSystem(world);
    movement.registerMovementSystem(world, displayWidth, displayHeight, timeStep);

    //Register Death system
    simbio::systems::Death death;
    death.registerDeathSystem(world);

    // Register flower reproduction system
    simbio::systems::FlowerReproduction flowerReproduction;
    flowerReproduction.registerFlowerReproductionSystem(world, timeStep, displayWidth, displayHeight);

    // Initialize queries for displaying organisms and plants
    auto drawBodyQuery = world.query<const Body, const Location>();
    auto drawLegsQuery = world.query<const Body, const Legs, const Location>();
    auto drawMouthQuery = world.query<const Body, const Mouth, const Location>();
    auto drawFlowersQuery = world.query<const Flower, const Location>();

    InitWindow(displayWidth, displayHeight, "SimBio");
    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < simSpeed; i++) world.progress(timeStep);

        drawBodyQuery.each([](const Body& body, const Location& location) {
            DrawCircleV({ location.x, location.y }, body.size * 0.5f, 
                Color{ body.color.r, body.color.g, body.color.b, 255 });
        });

        drawLegsQuery.each([](const Body& body, const Legs& legs, const Location& location) {
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

                DrawLineEx(start, end, legWidth, GREEN);
            }
        });


        drawMouthQuery.each([](const Body& body, const Mouth& mouth, const Location& location) {
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

            DrawLineEx(base, tip1, 4.0f, WHITE);
            DrawLineEx(base, tip2, 4.0f, WHITE);
        });

        drawFlowersQuery.each([](const Flower& flower, const Location& location) {
            DrawCircleV({ location.x, location.y }, flower.size * 0.5f, flower.color);
        });

        EndDrawing();
    }

    return 0;
}
