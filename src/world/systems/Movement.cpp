#include "Movement.h"
#include "organs/Legs.h"
#include "data/Data.h"
#include "Entity.h"
#include "raylib.h"
#include <algorithm>

namespace biosim {
    namespace systems {
        using namespace organism;

        void Movement::registerMoveIntentSystem(World& world) {
            world.flecsWorld.system<Legs, LegsRequest>()
                .each([&](flecs::entity e, const Legs& legs, LegsRequest& request) {
                float maxYaw = MAX_YAW * legs.size / Legs::MAX_SIZE;
                request.yaw = std::clamp(request.yaw, -maxYaw, maxYaw);
            });
        }

        void Movement::registerMovementSystem(World& world) {
            world.flecsWorld.system<LegsRequest, Location, Velocity, Legs, Status>()
                .each([&](flecs::entity e, LegsRequest& request, Location& location, 
                    Velocity& v, const Legs& legs, Status& status) {  

                float yawX = std::cos(location.yaw);
                float yawY = std::sin(location.yaw);
                
                float aX = request.a.x;
                float aY = request.a.y;

                request.a.x = aX * yawX - aY * yawY;
                request.a.y = aX * yawY + aY * yawX;

                float vMag = std::sqrt(v.x * v.x + v.y * v.y);
                if (vMag > 0.0f) {
                    float uVX = v.x / vMag;
                    float uVY = v.y / vMag;
                    float a = request.a.x * uVX + request.a.y * uVY;
                    float aX = a * uVX;
                    float aY = a * uVY;
                    if (a > 0.0f) {
                        float maxA = MAX_AXIS_A * legs.size / Legs::MAX_SIZE;
                        if (a > maxA) {
                            float scale = maxA / a;
                            aX *= scale;
                            aY *= scale;
                        }
                    } else if (a < 0.0f){
                        float maxDA = MAX_AXIS_DA * legs.size / Legs::MAX_SIZE;
                        if (a < maxDA) {
                            float scale = maxDA / a;
                            aX *= scale;
                            aY *= scale;
                        }
                    }
                    float turnX = request.a.x - aX;
                    float turnY = request.a.y - aY;
                    float maxTurnA = MAX_AXIS_TA * legs.size / Legs::MAX_SIZE;
                    float turnMag = std::sqrt(turnX * turnX + turnY * turnY);
                    if (turnMag > maxTurnA) {
                        float scale = maxTurnA / turnMag;
                        turnX *= scale;
                        turnY *= scale;
                    }
                    request.a.x = aX + turnX;
                    request.a.y = aY + turnY;
                }

                float vx0 = v.x;
                float vy0 = v.y;
                v.x += world.timeStep * request.a.x;
                v.y += world.timeStep * request.a.y;
                float s2 = v.x * v.x + v.y * v.y;
                float maxS = legs.size / Legs::MAX_SIZE * MAX_S;
                float s = std::sqrt(s2);
                if (s > maxS) {
                    float scale = maxS / s;
                    v.x *= scale;
                    v.y *= scale;
                    float dvx = v.x - vx0;
                    float dvy = v.y - vy0;
                    request.a.mag = std::sqrt(dvy * dvy + dvx * dvx) / world.timeStep;
                }

                location.x += world.timeStep * (vx0 + v.x) / 2.0f;
                location.x = location.x >= World::WIDTH ? 0.0f : location.x;
                location.x = location.x < 0.0f ? World::WIDTH - 0.0001f : location.x;
                

                location.y += world.timeStep * (vy0 + v.y) / 2.0f;
                location.y = location.y >= World::HEIGHT ? 0.0f : location.y;
                location.y = location.y < 0.0f ? World::HEIGHT - 0.0001f : location.y;
                
                float dYaw = request.yaw * world.timeStep;
                location.yaw += dYaw;
                status.energy -= YAW_ENERGY_COST * legs.size / Legs::MAX_SIZE * dYaw / MAX_YAW;
                if (location.yaw > PI || location.yaw <= -PI) 
                    location.yaw = std::atan2(std::sin(location.yaw), std::cos(location.yaw));

                status.energy -= MOVE_ENERGY_COST * legs.size / Legs::MAX_SIZE * s / maxS * world.timeStep;
                
                int newChunk = (int)(location.y / World::CHUNK_SIZE) * world.chunkCols 
                    + (int)(location.x / World::CHUNK_SIZE);
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
            });
        }
    }
}
