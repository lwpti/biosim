#pragma once

#include "flecs.h"
#include <vector>
#include "organs/Eyes.h"
#include "percepts/Sight.h"
#include "Entity.h"
#include "data/Status.h"

namespace simbio {
    namespace systems {
        class Vision {
        public:
            static void registerVisionSystem(std::vector<std::vector<organism::Entity>>& chunkGrid, 
                int chunkSize, int chunkCols, int chunkRows);
            
            static organism::Sight computeSightPercept(flecs::entity entity, float dt,
                const organism::Location& location, const organism::Eyes eyes, organism::Status status);    

            inline static std::vector<std::vector<organism::Entity>>* chunkGrid;
            inline static int chunkSize = 0;
            inline static int chunkCols = 0;
            inline static int chunkRows = 0;

            static constexpr float FOV = 140;
            static constexpr float RANGE_MULT = 16;
        };
    }
}
