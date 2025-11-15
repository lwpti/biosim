#include "flecs.h"
#include "Display.h"
#include "raylib.h"
#include "Organism.h"
#include "mover/Mover.h"

int main() {
    using namespace simbio::organism;

    flecs::world world;
    
    mover::Mover mover(world);
    mover.create().set<Location>({ 100, 100, 0 });
    mover.create().set<Location>({ 200, 200, 0 });
    mover.create().set<Location>({ 300, 300, 0 });

    const int displayWidth = 1000;
    const int displayHeight = 1000;

    simbio::systems::Display display(world, displayWidth, displayHeight);

    InitWindow(displayWidth, displayHeight, "SimBio");
    SetTargetFPS(30);


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        world.progress();

        EndDrawing();
    }

    return 0;
}