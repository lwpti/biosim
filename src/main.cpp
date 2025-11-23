#include "flecs.h"
#include "Organism.h"
#include "raylib.h"
#include "organism/data/Velocity.h"
#include "organism/organisms/mover/Mover.h"
#include "world/systems/Movement.h"
#include <cmath>

int main() {
    using namespace simbio::organism;

    flecs::world world;
    
    mover::Mover mover(world);
    mover.create().set<Location>({ 100, 100, 0 }).set<Velocity>({ 0, 0 });
    mover.create().set<Location>({ 200, 200, 0 }).set<Velocity>({ 0, 0 });
    mover.create().set<Location>({ 300, 300, 0 }).set<Velocity>({ 0, 0 });

    const int displayWidth = 1000;
    const int displayHeight = 1000;
    simbio::systems::Movement movement;
    movement.registerMoveIntentSystem(world);
    movement.registerMovementSystem(world, displayWidth, displayHeight);

    auto drawBodyQuery = world.query<const Body, const Location>();
    auto drawLegsQuery = world.query<const Body, const Legs, const Location>();

    InitWindow(displayWidth, displayHeight, "SimBio");
    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < 20; i++) world.progress(0.1);

        drawBodyQuery.each([](const Body& body, const Location& location) {
            DrawCircleV(
                { (float)location.x, (float)location.y },
                body.size * 0.5f,
                RED
            );
            });

        drawLegsQuery.each([](const Body& body, const Legs& legs, const Location& location) {
            float x = location.x;
            float y = location.y;
            float radius = body.size * 0.5f;
            const int numLegs = 8;
            float legLength = legs.size;
            float legWidth = legLength / 6.0f;

            for (int i = 0; i < numLegs; ++i) {
                float angle = (2.0f * PI * i) / numLegs;

                Vector2 start = { x + std::cos(angle) * radius, y + std::sin(angle) * radius };

                Vector2 end = { x + std::cos(angle) * (radius + legLength), y + std::sin(angle) * (radius + legLength) };

                DrawLineEx(start, end, legWidth, GREEN);
            }
            });

        EndDrawing();
    }

    return 0;
}