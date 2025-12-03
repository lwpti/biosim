#pragma once

#include "flecs.h"
#include <vector>
#include "Organism.h"
#include "Entity.h"

namespace simbio {
    namespace systems {
        class Vision {
        public:
            static void registerVisionSystem(std::vector<std::vector<organism::Entity>>& chunkGrid, 
                int chunkSize, int chunkCols);
            
            static organism::Sight computeSightPercept(flecs::entity entity, 
                const organism::Location& location, const organism::Eyes eyes);    

            inline static std::vector<std::vector<organism::Entity>>* chunkGrid;
            inline static int chunkSize = 0;
            inline static int chunkCols = 0;

            static constexpr float FOV = 140;
            static constexpr float RANGE_MULT = 16;
        };
    }
}
