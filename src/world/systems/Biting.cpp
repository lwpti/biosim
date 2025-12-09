#include "Biting.h"

#include <cmath>
#include <algorithm>

#include "data/Location.h"
#include "data/Status.h"
#include "organs/Flower.h"
#include "organs/Arms.h"
#include "organs/Mouth.h"
#include "percepts/Touch.h"
#include "Entity.h"

namespace simbio {
    namespace systems {

        using namespace organism;

        void Biting::registerBitingSystem(World& world)
        {
            world.flecsWorld.system<Mouth, BiteIntent, Location, Status>("BitingSystem").each(
                [&](flecs::entity entity, Mouth& mouth, BiteIntent biteIntent, Location& location, Status& status) {
                    status.energy -= 0.5f;

                    const float range = mouth.size * 0.4f;
                    const float range2 = range * range;

                    Entity bestTarget;
                    float bestDist2 = range2;

                    world.forNearbyEntities(location, [&](Entity& target) {
                        if (entity.id() == target.flecsID) return;

                        const float dx = target.location.x - location.x;
                        const float dy = target.location.y - location.y;
                        const float dist2 = dx * dx + dy * dy;

                        if (dist2 < bestDist2) {
                            bestDist2 = dist2;
                            bestTarget = target;
                        }
                    });

                    if (bestDist2 == range2) return;

                    flecs::entity target(world.flecsWorld, bestTarget.flecsID);

                    if (target.has<Flower>()) {
                        status.energy = std::min(1500.0f, status.energy + mouth.size * 20.0f);
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
                    } else if (target.has<Status>()) {
                        Status& targetStatus = target.get_mut<Status>();
                        targetStatus.health -= mouth.size * 40.0f;
                        status.energy += mouth.size * 40.0f;
                        return;
                    }
                }
            );      
        }
    } 
} 
