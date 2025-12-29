#include "Mouth.h"
#include "World.h"
#include "Body.h"

namespace biosim {
    namespace organism {
        void Mouth::registerMouthObserver(World& world) {
            world.flecsWorld.observer<Mouth>().event(flecs::OnSet).each(
                [](flecs::entity e, Mouth& mouth) {
                    if (mouth.size < MIN_SIZE) {
                        mouth.size = MIN_SIZE;
                    }
                    else {
                        const Body& body = e.get<Body>();
                        float maxSize = std::min(body.size * MAX_OVERSIZE, MAX_SIZE);
                        if (mouth.size > maxSize) {
                            mouth.size = maxSize;
                        }
                    }
                    if (mouth.power < MIN_POWER) {
                        mouth.power = MIN_POWER;
                    }
                    else if (mouth.power > MAX_POWER) {
                        mouth.power = MAX_POWER;
                    }
                }
            );
        }
    }
}
