#pragma once

#include "Arms.h"
#include "Body.h"
#include "Ears.h"
#include "Eyes.h"
#include "Legs.h"
#include "Mouth.h"
#include "Flower.h"
#include <concepts>
#include "flecs.h"

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
        struct Organs {
            Body body{ 0.0f };
            Legs legs{ 0.0f };
            Mouth mouth{ 0.0f };
            Arms arms{ 0.0f };
            Ears ears{ 0.0f };
            Eyes eyes{ 0.0f };
            Flower flower{ 0.0f };

        private:
            template <Organ O>
            static void registerOrganObserver(flecs::world& world) {
                world.observer<O>()
                    .event(flecs::OnSet)
                    .each([](flecs::entity e, O& organ) {
                            if (organ.size < O::MIN_SIZE) {
                                organ.size = O::MIN_SIZE;
                            }
                            else if (organ.size > O::MAX_SIZE) {
                                organ.size = O::MAX_SIZE;
                            }
                    });
            }

        public:
            static void registerOrganObservers(flecs::world& world) {
                Organs::registerOrganObserver<Arms>(world);
                Organs::registerOrganObserver<Body>(world);
                Organs::registerOrganObserver<Ears>(world);
                Organs::registerOrganObserver<Eyes>(world);
                Organs::registerOrganObserver<Flower>(world);
                Organs::registerOrganObserver<Legs>(world);
                Organs::registerOrganObserver<Mouth>(world);
            }
        };
    }
}