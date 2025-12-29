#pragma once

#include <concepts>
#include "organs/Organs.h"
#include "World.h"

namespace biosim {
    namespace organism {
        template<typename T>
        concept Organ = requires(T t) {
            requires std::same_as<decltype(t.size), float>;
            requires std::same_as<decltype(T::MIN_SIZE), const float>;
            requires std::same_as<decltype(T::MAX_SIZE), const float>;
        };

        /// <summary>
        /// Convenient structure grouping all organ components.
        /// Organ.size = 0 means the organ is not present.
        /// </summary>
        struct OrganObserver {
            Body body{ 0.0f };
            Legs legs{ 0.0f };
            Mouth mouth{ 0.0f };
            Arms arms{ 0.0f };
            Ears ears{ 0.0f };
            Eyes eyes{ 0.0f };
            Flower flower{ 0.0f };

            private:
            template <Organ O>
            static void registerOrganObserver(World& world, float maxOversize) {
                world.flecsWorld.observer<O>().event(flecs::OnSet).each(
                    [maxOversize](flecs::entity e, O& organ) {
                        if (organ.size < O::MIN_SIZE) {
                            organ.size = O::MIN_SIZE;
                        }
                        else {
                            float maxSize = O::MAX_SIZE;
                            const Body* body = e.try_get<Body>();
                            if (body) maxSize = std::min(body->size * maxOversize, maxSize);
                            if (organ.size > maxSize) {
                                organ.size = maxSize;
                            }
                        }
                    }
                );
            }

            public:
            static void registerOrganObservers(World& world) {
                OrganObserver::registerOrganObserver<Arms>(world, 1.5f);
                OrganObserver::registerOrganObserver<Body>(world, 1.0f);
                OrganObserver::registerOrganObserver<Ears>(world, 2.5f);
                OrganObserver::registerOrganObserver<Eyes>(world, 2.5f);
                OrganObserver::registerOrganObserver<Flower>(world, 1.5f);
                Mouth::registerMouthObserver(world);
                Legs::registerLegsObserver(world);
            }
        };
    }
}