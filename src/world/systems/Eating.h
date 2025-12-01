#pragma once

#include "flecs.h"
#include <vector>
#include "Organism.h"

namespace simbio {
    namespace systems {

        class Eating {
        public:
            /// Register a system that processes ArmsIntent and creates Holding/Grabbed relations
            void registerGrabbingSystem(
                flecs::world& world,
                std::vector<std::vector<organism::SimpleEntity>>& chunkGrid,
                int chunkSize,
                int chunkCols,
                int chunkRows);

            /// Register a system that processes BiteIntent and applies damage / eats plants
            void registerBitingSystem(
                flecs::world& world,
                std::vector<std::vector<organism::SimpleEntity>>& chunkGrid,
                int chunkSize,
                int chunkCols,
                int chunkRows);
        };

    }
}
