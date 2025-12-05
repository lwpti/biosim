#include "Movement.h"
#include "organs/Legs.h"
#include "data/Data.h"
#include "Entity.h"
#include "raylib.h"

namespace simbio {
    namespace systems {
        using namespace organism;

        void Movement::registerMoveIntentSystem(World& world) {
            world.flecsWorld.system<Legs, LegsIntent>("MoveIntentSystem")
                .each([](flecs::entity e, const Legs& legs, const LegsIntent& legsIntent) 
                    {
                        float maxYaw = 2.0f * PI * legs.size / 6.0f;

                        float ax = legsIntent.a.x;
                        float ay = legsIntent.a.y;
                        float a = std::sqrt(ax * ax + ay * ay);
                        if (a > legs.size) {
                            float scale = legs.size / a;
                            a = legs.size;
							ax *= scale;
							ay *= scale;
                        }

                        e.set<Move>({ { a, ax, ay }, std::clamp(legsIntent.yaw, -maxYaw, maxYaw) });
                        e.remove<LegsIntent>();
                    });
        }

        void Movement::registerMovementSystem(World& world) {
            world.flecsWorld.system<Move, Location, Velocity, Legs, Status>("MovementSystem")
                .each([&]
                    (flecs::entity e, Move& move, Location& location, Velocity& v, const Legs& legs, Status& status)
                    {
                        // TODO: Improve drag.
                        v.x *= (1.0f - 0.1 * world.timeStep);
                        v.y *= (1.0f - 0.1 * world.timeStep);
						float vx0 = v.x;
						float vy0 = v.y;
						v.x += world.timeStep * move.a.x;
                        v.y += world.timeStep * move.a.y;
						float s2 = v.x * v.x + v.y * v.y;
						float sMax = legs.size * 2.0f;
                        if (s2 > sMax * sMax) {
                            float s = std::sqrt(s2);
                            float scale = sMax / s;
                            v.x *= scale;
                            v.y *= scale;
                            float dvx = v.x - vx0;
							float dvy = v.y - vy0;
							move.a.magnitude = std::sqrt(dvy * dvy + dvx * dvx) / world.timeStep;
                        }

                        location.x += world.timeStep * (vx0 + v.x) / 2.0f;
                        location.x = location.x >= world.width ? 0.0f : location.x;
                        location.x = location.x < 0.0f ? world.width - 0.0001f : location.x;
                        

                        location.y += world.timeStep * (vy0 + v.y) / 2.0f;
                        location.y = location.y >= world.height ? 0.0f : location.y;
                        location.y = location.y < 0.0f ? world.height - 0.0001f : location.y;

                        location.yaw += move.yaw * world.timeStep;
                        if (location.yaw > PI || location.yaw <= -PI) 
                            location.yaw = std::atan2(std::sin(location.yaw), std::cos(location.yaw));

                        if (move.a.magnitude > 0.6f * legs.size) {
							float overA = move.a.magnitude - 0.6f * legs.size;
							status.energy -= (0.6f * legs.size + (overA * overA)) / 10.0f * world.timeStep;
                        }
                        else {
                            // Max A = legs.size
                            status.energy -= move.a.magnitude / 10.0f * world.timeStep;
                        }
                        
                        int newChunk = (int)(location.y / World::CHUNK_SIZE) * world.chunkCols + (int)(location.x / World::CHUNK_SIZE);
                        auto& bucket = world.chunkGrid[location.chunk];
                        for (int i = 0; i < bucket.size(); ++i) {
                            if (bucket[i].flecsID == e.id()) {
                                bucket[i].location = location;
                                bucket[i].velocity = v;
                                if (newChunk != location.chunk) {
                                    location.chunk = newChunk;
                                    bucket[i].location.chunk = newChunk;
                                    world.chunkGrid[newChunk].push_back(bucket[i]);
                                    bucket[i] = bucket.back();
                                    bucket.pop_back();
                                    break;
                                }
                            }
                        }

                        e.remove<Move>();
                    });
        }
    }
}
