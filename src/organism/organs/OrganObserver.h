#pragma once

#include <concepts>
#include "organs/Organs.h"
#include "World.h"

namespace simbio {
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
            static void registerOrganObserver(World& world) {
                world.flecsWorld.observer<O>().event(flecs::OnSet).each(
                    [](flecs::entity e, O& organ) {
                        if (organ.size < O::MIN_SIZE) {
                            organ.size = O::MIN_SIZE;
                        }
                        else if (organ.size > O::MAX_SIZE) {
                            organ.size = O::MAX_SIZE;
                        }
                    }
                );
            }

        public:
            static void registerOrganObservers(World& world) {
                OrganObserver::registerOrganObserver<Arms>(world);
                OrganObserver::registerOrganObserver<Body>(world);
                OrganObserver::registerOrganObserver<Ears>(world);
                OrganObserver::registerOrganObserver<Eyes>(world);
                OrganObserver::registerOrganObserver<Flower>(world);
                OrganObserver::registerOrganObserver<Legs>(world);
                OrganObserver::registerOrganObserver<Mouth>(world);
            }
        };
    }
}