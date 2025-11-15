#include "flecs.h"
#include "Display.h"
#include "raylib.h"
#include "Organism.h"

int main() {
    using namespace simbio::organism;

    flecs::world world;
    
    world.entity()
        .set<Body>({ 10 })
        .set<Legs>({ 10 })
        .set<Location>({ 100, 100, 0 });

    world.entity()
        .set<Body>({ 20 })
        .set<Legs>({ 10 })
        .set<Location>({ 200, 200, 0 });

    world.entity()
        .set<Body>({ 5 })
        .set<Legs>({ 10 })
        .set<Location>({ 300, 300, 0 });

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