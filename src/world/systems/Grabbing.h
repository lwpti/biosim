#pragma once

#include "World.h"

namespace simbio {
    namespace systems {
            class Grabbing {
            /// <summary>
            /// Registers system that processes ArmsIntent and performs grabbing.
            /// One grabber can grab at most one target, but a target can be grabbed
            /// by multiple grabbers. Grabs only work in a 180 cone in front.
            /// </summary>
            void registerGrabbingSystem(World& world);
        };
    }
}