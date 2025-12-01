#pragma once

#include "flecs.h"
#include <vector>
#include "Organism.h"

namespace simbio {
    namespace systems {

        class Vision {
        public:
            void registerVisionSystem(
                flecs::world& world,
                std::vector<std::vector<organism::SimpleEntity>>& chunkGrid,
                int chunkSize,
                int chunkCols,
                int chunkRows);
        };

    }
}
