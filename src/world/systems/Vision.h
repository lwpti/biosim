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
            
            static organism::Sight computeSightPercept(flecs::entity entity, float dt,
                const organism::Location& location, const organism::Eyes eyes, organism::Status status);    
            
            static World* world;
            
            static constexpr float FOV = 140;
            static constexpr float RANGE_MULT = 16;
        };
    }
}
