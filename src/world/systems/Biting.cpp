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
            world.flecsWorld.system<Mouth, Body, BiteIntent, Location, Status>().kind(flecs::PostUpdate).each(
                [&](flecs::entity entity, const Mouth& mouth, const Body& body, 
                    BiteIntent& biteIntent, const Location& location, Status& status) {
                    if (biteIntent.strength <= 0.0f) return;
                    float strength = std::clamp(biteIntent.strength, 0.0f, 1.0f);
                    biteIntent.strength = 0.0f;
                    status.energy -= BITE_ENERGY_COST * mouth.size / Mouth::MAX_SIZE 
                        * mouth.power / Mouth::MAX_POWER * strength * world.timeStep;

                    const float minDist2 = body.size * body.size * 0.25f;
                    const float reach = body.size * 0.5f + mouth.size;
                    const float maxDist2 = reach * reach;

                    float forwardX = std::cos(location.yaw);
                    float forwardY = std::sin(location.yaw);

                    world.forNearbyEntities(location, [&](Entity& target) {
                        if (entity.id() == target.flecsID) return;

                        const float distX = target.location.x - location.x;
                        const float distY = target.location.y - location.y;
                        const float dist2 = distX * distX + distY * distY;

                        if (dist2 <= minDist2) return;
                        if (dist2 >= maxDist2) return;

                        // Note: this is dot * dist, technically
                        float dot = forwardX * distX + forwardY * distY;

                        if (dot <= 0.0f) return; 
                        if (dot * dot < MIN_BITE_DOT2 * dist2) return;

                        flecs::entity targetEntity(world.flecsWorld, target.flecsID);

                        if (Flower* flower = targetEntity.try_get_mut<Flower>()) {
                            status.energy += flower->size * Status::FLOWER_ENERGY;
                            status.energy = std::min(Status::MAX_ENERGY * body.size / Body::MAX_SIZE, status.energy);
                            auto& bucket = world.chunkGrid[target.location.chunk];
                            for (int i = 0; i < bucket.size(); ++i) {
                                if (bucket[i].flecsID == target.flecsID) {
                                    bucket[i] = bucket.back();
                                    bucket.pop_back();
                                    break;
                                }
                            }
                            targetEntity.destruct();
                        } else if (Status* targetStatus = targetEntity.try_get_mut<Status>()) {
                            float damage = mouth.power * mouth.size * world.timeStep * strength;
                            // Damage < 0.1 does not break the skin (1.0f * timeStep)
                            if (damage < world.timeStep) return;
                            targetStatus->health -= damage;
                            status.energy += damage * Status::BITE_ENERGY;
                        }
                    });
                }
            );      
        }
    } 
} 
