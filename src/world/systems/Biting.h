#pragma once

#include "World.h"
#include "Constants.h"

namespace biosim {
    namespace systems {
        class Biting {
        public:
            /// <summary>
            /// Registers system that processes BiteIntent and performs biting /
            /// killing behavior on nearby entities.
            /// </summary>
            static void registerBitingSystem(World& world);

            static constexpr float MAX_BITE_ANGLE = PI_F / 12.0f;
            // cos(PI / 12)^2, results in 15 degree range in each direction, AKA 30 degree arc 
            inline static const float MIN_BITE_DOT2 = std::cos(MAX_BITE_ANGLE) * std::cos(MAX_BITE_ANGLE);
            // Mouth consumes ~11% of energy if used continuously, and total usage ~= 2/s
            static constexpr float BITE_ENERGY_COST = 0.22f;
        };
    }
}


