#include "Biting.h"

#include <cmath>
#include <algorithm>

#include "data/Location.h"
#include "data/Status.h"
#include "organs/Flower.h"
#include "organs/Mouth.h"
#include "Entity.h"

namespace simbio {
    namespace systems {

        using namespace organism;

        void Biting::registerBitingSystem(World& world) {
            world.flecsWorld.system<Mouth, Body, BiteIntent, Location, Status>().each(
                [&](flecs::entity entity, const Mouth& mouth, const Body& body, 
                    const BiteIntent& biteIntent, const Location& location, Status& status) {
                    status.energy -= BITE_ENERGY_COST * mouth.size / Mouth::MAX_SIZE 
                        * mouth.power / Mouth::MAX_POWER * biteIntent.strength;

                    const float minDist2 = body.size * body.size / 4.0f;
                    const float reach = body.size + mouth.size;
                    const float maxDist2 = reach * reach;

                    float forwardX = std::cos(location.yaw);
                    float forwardY = std::sin(location.yaw);

                    Entity bestTarget;
                    float bestDist2 = maxDist2;

                    world.forNearbyEntities(location, [&](Entity& target) {
                        if (entity.id() == target.flecsID) return;

                        const float distX = target.location.x - location.x;
                        const float distY = target.location.y - location.y;
                        const float dist2 = distX * distX + distY * distY;

                        if (dist2 <= minDist2) return;
                        if (dist2 >= bestDist2) return;

                        // Note: this is dot * dist, technically
                        float dot = forwardX * distX + forwardY * distY;

                        if (dot <= 0.0f) return; 
                        if (dot * dot < MIN_BITE_DOT2 * dist2) return;

                        bestDist2 = dist2;
                        bestTarget = target;
                    });

                    if (bestDist2 == maxDist2) return;

                    flecs::entity target(world.flecsWorld, bestTarget.flecsID);

                    if (Flower* flower = target.try_get_mut<Flower>()) {
                        status.energy += flower->size * Status::FLOWER_ENERGY;
                        status.energy = std::min(Status::MAX_ENERGY * body.size / Body::MAX_SIZE, status.energy);
                        auto& bucket = world.chunkGrid[bestTarget.location.chunk];
                        for (int i = 0; i < bucket.size(); ++i) {
                            if (bucket[i].flecsID == bestTarget.flecsID) {
                                bucket[i] = bucket.back();
                                bucket.pop_back();
                                break;
                            }
                        }
                        target.destruct();
                        return;
                    } else if (Status* targetStatus = target.try_get_mut<Status>()) {
                        float strength = std::clamp(biteIntent.strength, 0.0f, 1.0f);
                        float damage = mouth.power * mouth.size * world.timeStep * strength;
                        // Damage < 0.1 does not break the skin (1.0f * timeStep)
                        if (damage < world.timeStep) return;
                        targetStatus->health -= damage;
                        status.energy += damage * Status::BITE_ENERGY;
                        return;
                    }
                }
            );      
        }
    } 
} 
