#include "raylib.h"

int main() {
    constexpr int displayWidth = 1000;
    constexpr int displayHeight = 1000;

    InitWindow(displayWidth, displayHeight, "SimBio");

    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        for (int x = 0; x <= displayWidth; x += 10) {
            DrawLine(x, 0, x, displayHeight, DARKGRAY);
        }
        for (int y = 0; y <= displayWidth; y += 10) {
            DrawLine(0, y, displayHeight, y, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}