#pragma once

#include "Arms.h"
#include "Body.h"
#include "Ears.h"
#include "Eyes.h"
#include "Legs.h"
#include "Mouth.h"
#include "Flower.h"

#include <optional>

namespace biosim {
    namespace organism {
        /// <summary>
        /// Convenient structure grouping all organ components.
        /// A null optional means the organ is not present.
        /// </summary>
        struct Organs {
            std::optional<Body> body;
            std::optional<Legs> legs;
            std::optional<Mouth> mouth;
            std::optional<Arms> arms;
            std::optional<Ears> ears;
            std::optional<Eyes> eyes;
            std::optional<Flower> flower;

            template <typename Organ>
            static float sizeOf(const std::optional<Organ>& organ) {
                return organ.has_value() ? organ->size : 0.0f;
            }
        };
    }
}