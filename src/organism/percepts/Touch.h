#pragma once

#include <flecs.h>
#include "data/Location.h"

namespace biosim {
    namespace organism {

        // Component added to the grabber: "I am holding this entity"
        struct Holding {
            flecs::entity_t targetId;
        };

        // Component added to the grabbed: "I am held by this entity"
        struct Grabbed {
            flecs::entity_t holderId;
        };

    }
}
