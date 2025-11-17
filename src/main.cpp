#include "flecs.h"
#include "raylib.h"
#include "Organism.h"
#include "world/systems/Display.h"
#include "world/systems/Movement.h"
#include "organism/data/Velocity.h"
#include "organism/organisms/mover/Mover.h"

int main() {
    using namespace simbio::organism;

    flecs::world world;
    
    mover::Mover mover(world);
    mover.create().set<Location>({ 100, 100, 0 }).set<Velocity>({ 0, 0 });
    mover.create().set<Location>({ 200, 200, 0 }).set<Velocity>({ 0, 0 });
    mover.create().set<Location>({ 300, 300, 0 }).set<Velocity>({ 0, 0 });

    simbio::systems::Movement movement;
    movement.registerMoveIntentSystem(world);
    movement.registerMovementSystem(world);

    const int displayWidth = 1000;
    const int displayHeight = 1000;
    simbio::systems::Display(displayWidth, displayHeight).registerDrawSystems(world);

    InitWindow(displayWidth, displayHeight, "SimBio");
    //SetTargetFPS(300);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        world.progress();

        EndDrawing();
    }

    return 0;
}