#pragma once

#include <flecs.h>
#include "data/Location.h"

namespace simbio {
    namespace organism {

        // What this organism currently sees (simple version: a single target)
        struct SightPercept {
            Location targetLocation;
            flecs::entity_t targetId;
            bool hasTarget;
        };

    }
}
