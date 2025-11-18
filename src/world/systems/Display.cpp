#include "raylib.h"
#include <cmath>
#include "organs/Organs.h"
#include "data/Location.h"
#include "Display.h"

namespace simbio {
    namespace systems {
        Display::Display(int width, int height) : width(width), height(height) {}

        void Display::registerDrawSystems(flecs::world& world) const {
            using namespace simbio::organism;

            world.system<Body, Location>("DrawBodySystem")
                .each([](const Body& body, const Location& location) 
                    {
                        DrawCircleV({ (float)location.x, (float)location.y }, body.size * 0.5f, RED);
                    });

            world.system<Body, Legs, Location>("DrawLegsSystem")
                .each([](const Body& body, const Legs& legs, const Location& location) 
                    {
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
        }
    }
}
