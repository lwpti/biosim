#include "flecs.h"
#include "Organism.h"
#include "raylib.h"
#include "organism/data/Velocity.h"
#include "organism/organisms/mover/Mover.h"
#include "world/systems/Movement.h"
#include "world/systems/Death.h"
#include <cmath>
#include "data/Status.h"
#include "plants/Flower.h"

int main() {
    using namespace simbio::organism;
    using namespace simbio::plants;

    flecs::world world;
    
    // Spawn some Mover entities
    mover::Mover mover(world);
    mover.create().set<Location>({ 100, 100, 0 }).set<Velocity>({ 0, 0 }).set<Status>({ 100, 100});
    mover.create().set<Location>({ 200, 200, 0 }).set<Velocity>({ 0, 0 }).set<Status>({ 100, 100 });
    mover.create().set<Location>({ 300, 300, 0 }).set<Velocity>({ 0, 0 }).set<Status>({ 100, 100 });

    // Spawn some Flowers
    world.entity().set<Flower>({ 5 }).set<Location>({ 150, 150, 0 });
    world.entity().set<Flower>({ 5 }).set<Location>({ 250, 250, 0 });
    world.entity().set<Flower>({ 5 }).set<Location>({ 350, 350, 0 });

    // Register MoveIntent and Movement systems
    const int displayWidth = 1000;
    const int displayHeight = 1000;
    simbio::systems::Movement movement;
    movement.registerMoveIntentSystem(world);
    movement.registerMovementSystem(world, displayWidth, displayHeight);

    //Register Death system
    simbio::systems::Death death;
    death.registerDeathSystem(world);

    // Initialize queries for displaying organisms and plants
    auto drawBodyQuery = world.query<const Body, const Location>();
    auto drawLegsQuery = world.query<const Body, const Legs, const Location>();
    auto drawFlowersQuery = world.query<const Flower, const Location>();

    InitWindow(displayWidth, displayHeight, "SimBio");
    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < 20; i++) world.progress(0.1);

        drawBodyQuery.each([](const Body& body, const Location& location) {
            DrawCircleV({ location.x, location.y }, body.size * 0.5f, RED);
            });

        drawLegsQuery.each([](const Body& body, const Legs& legs, const Location& location) {
            float radius = body.size * 0.5f;
            const int numLegs = 8;
            float legWidth = legs.size / 6.0f;

            for (int i = 0; i < numLegs; ++i) {
                float angle = (2.0f * PI * i) / numLegs;
                Vector2 start = { location.x + std::cos(angle) * radius, location.y + std::sin(angle) * radius };
                Vector2 end = { location.x + std::cos(angle) * (radius + legs.size), location.y + std::sin(angle) * (radius + legs.size) };
                DrawLineEx(start, end, legWidth, GREEN);
            }
            });

        drawFlowersQuery.each([](const Flower& flower, const Location& location) {
            DrawCircleV({ location.x, location.y }, flower.size * 0.5f, GREEN);
            });

        EndDrawing();
    }

    return 0;
}