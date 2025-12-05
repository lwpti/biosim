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
        };
    }
}


