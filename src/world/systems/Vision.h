#pragma once

#include "organs/Eyes.h"
#include "percepts/Sight.h"
#include "Entity.h"
#include "data/Status.h"
#include "data/Location.h"
#include "World.h"

namespace simbio {
    namespace systems {
        class Vision {
            public:
            static void registerVisionSystem(World& world);
            
            static constexpr float FOV = 140;
            static constexpr float RANGE_MULT = 16;
            // Eyes consumes ~1% of energy if used continuously, and total usage ~= 2/s
            static constexpr float VISION_ENERGY_COST = 0.02f;
        };
    }
}
