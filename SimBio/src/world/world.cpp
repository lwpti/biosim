#include "flecs.h"
#include "Display.h"
#include "raylib.h"
#include "Organism.h"

using namespace std;

int main() {
    flecs::world world;
    
    world.entity()
        .set<simbio::organism::Body>({ 10 })
        .set<simbio::organism::Legs>({ 10 })
        .set<simbio::organism::Location>({ 100, 100, 0 });

    world.entity()
        .set<simbio::organism::Body>({ 20 })
        .set<simbio::organism::Legs>({ 10 })
        .set<simbio::organism::Location>({ 200, 200, 0 });

    world.entity()
        .set<simbio::organism::Body>({ 5 })
        .set<simbio::organism::Legs>({ 10 })
        .set<simbio::organism::Location>({ 300, 300, 0 });

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