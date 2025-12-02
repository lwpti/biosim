#include "Vision.h"

namespace simbio {
    namespace systems {

        void Vision::registerVisionSystem(
            flecs::world& world,
            std::vector<std::vector<organism::Entity>>& chunkGrid,
            int chunkSize,
            int chunkCols,
            int chunkRows)
        {
            // no-op stub for now
            (void)world;
            (void)chunkGrid;
            (void)chunkSize;
            (void)chunkCols;
            (void)chunkRows;
        }

    }
}
