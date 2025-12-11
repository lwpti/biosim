#include "Legs.h"
#include "World.h"
#include "Body.h"

namespace simbio {
    namespace organism {
        void Legs::registerLegsObserver(World& world) {
            world.flecsWorld.observer<Legs>().event(flecs::OnSet).each(
                [](flecs::entity e, Legs& legs) {
                    if (legs.size < MIN_SIZE) {
                        legs.size = MIN_SIZE;
                    }
                    else {
                        const Body& body = e.get<Body>();
                        float maxSize = std::min(body.size * MAX_OVERSIZE, MAX_SIZE);
                        if (legs.size > maxSize) {
                            legs.size = maxSize;
                        }
                    }
                    // Soft power cap = 0.8 (at 25% of total power)
                    float totalPower = legs.forwardPower + legs.backwardPower + legs.leftwardPower + legs.rightwardPower;
                    if (totalPower == 0.0f) totalPower = 1.0f;
                    legs.forwardPower = legs.forwardPower / totalPower;
                    legs.forwardPower = legs.forwardPower > 0.25f ? 
                        0.8f + 0.267f * (legs.forwardPower - 0.25f) : 3.2f * legs.forwardPower;
                    legs.backwardPower = legs.backwardPower / totalPower;
                    legs.backwardPower = legs.backwardPower > 0.25f ? 
                        0.8f + 0.267f * (legs.backwardPower - 0.25f) : 3.2f * legs.backwardPower;
                    legs.leftwardPower = legs.leftwardPower / totalPower;
                    legs.leftwardPower = legs.leftwardPower > 0.25f ? 
                        0.8f + 0.267f * (legs.leftwardPower - 0.25f) : 3.2f * legs.leftwardPower;
                    legs.rightwardPower = legs.rightwardPower / totalPower;
                    legs.rightwardPower = legs.rightwardPower > 0.25f ? 
                        0.8f + 0.267f * (legs.rightwardPower - 0.25f) : 3.2f * legs.rightwardPower;
                }
            );
        }
    }
}
