#include <cmath>
#include "Movement.h"
#include "organs/Legs.h"
#include "data/Location.h"
#include "data/Velocity.h"
#include "raymath.h"
#include <data/Status.h>
#include <algorithm>

namespace simbio {
    namespace systems {
        using namespace organism;

        void Movement::registerMoveIntentSystem(flecs::world& world) {
            world.system<Legs, LegsIntent>("MoveIntentSystem")
                .each([](flecs::entity e, const Legs& legs, const LegsIntent& legsIntent) 
                    {
                        float maxYaw = 2.0f * PI * legs.size / 6.0f;

                        float ax = legsIntent.a.x;
                        float ay = legsIntent.a.y;
                        float a = std::sqrt(ax * ax + ay * ay);
                        if (a > legs.size) {
                            float scale = legs.size / a;
							ax *= scale;
							ay *= scale;
                            a = legs.size;
                        }

                        e.set<Move>({ { a, ax, ay }, std::clamp(legsIntent.yaw, -maxYaw, maxYaw) });
                        e.remove<LegsIntent>();
                    });
        }

        void Movement::registerMovementSystem(flecs::world& world, int worldWidth, int worldHeight, 
				float dt, std::vector<std::vector<organism::Entity>>& chunkGrid, int chunkSize, int chunkCols) {
            world.system<Move, Location, Velocity, Legs, Status>("MovementSystem")
                .each([&, worldWidth, worldHeight, dt, chunkSize, chunkCols]
                    (flecs::entity e, Move& move, Location& location, Velocity& v, const Legs& legs, Status& status)
                    {
                        // TODO: Improve drag.
                        v.x *= (1.0f - 0.1 * dt);
                        v.y *= (1.0f - 0.1 * dt);
						float vx0 = v.x;
						float vy0 = v.y;
						v.x += dt * move.a.x;
                        v.y += dt * move.a.y;
						float s2 = v.x * v.x + v.y * v.y;
						float sMax = legs.size * 2.0f;
                        if (s2 > sMax * sMax) {
                            float s = std::sqrt(s2);
                            float scale = sMax / s;
                            v.x *= scale;
                            v.y *= scale;
                            float dvx = v.x - vx0;
							float dvy = v.y - vy0;
							move.a.magnitude = std::sqrt(dvy * dvy + dvx * dvx) / dt;
                        }

                        location.x += dt * (vx0 + v.x) / 2.0f;
                        location.x = location.x >= worldWidth ? 0.0f : location.x;
                        location.x = location.x < 0.0f ? worldWidth - 0.0001f : location.x;
                        

                        location.y += dt * (vy0 + v.y) / 2.0f;
                        location.y = location.y >= worldHeight ? 0.0f : location.y;
                        location.y = location.y < 0.0f ? worldHeight - 0.0001f : location.y;

                        location.yaw += move.yaw;

                        if (move.a.magnitude > 0.6f * legs.size) {
							float overA = move.a.magnitude - 0.6f * legs.size;
							status.energy -= (0.6f * legs.size + (overA * overA)) / 100.0f;
                        }
                        else {
                            status.energy -= move.a.magnitude / 100.0f;
                        }
                        
                        int newChunk = (int)(location.y / chunkSize) * chunkCols + (int)(location.x / chunkSize);
                        if (newChunk != location.chunk) {
                            auto& bucket = chunkGrid[location.chunk];
                            for (int i = 0; i < bucket.size(); ++i) {
                                if (bucket[i].flecsID == e.id()) {
                                    location.chunk = newChunk;
                                    bucket[i].location = location;
                                    bucket[i].velocity = v;
                                    break;
                                }
                            }
                        }

                        e.remove<Move>();
                    });
        }
    }
}
