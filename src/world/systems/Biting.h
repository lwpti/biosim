#pragma once

#include "World.h"

namespace simbio {
    namespace systems {
        class Biting {
        public:
            /// <summary>
            /// Registers system that processes BiteIntent and performs biting /
            /// killing behavior on nearby entities.
            /// </summary>
            void registerBitingSystem(World& world);

            // cos(PI / 12)^2, results in 15 degree range in each direction, AKA 30 degree arc 
            static constexpr float MIN_BITE_DOT2 = 0.933012702f;
            // Mouth consumes ~11% of energy if used continuously, and total usage ~= 2/s
            static constexpr float BITE_ENERGY_COST = 0.22f;
        };
    }
}


