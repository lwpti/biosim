#pragma once

#include <flecs.h>
#include "data/Location.h"
#include "organs/Organs.h"
#include "data/Status.h"
#include "data/Color.h"
#include "data/Velocity.h"
#include <vector>

namespace simbio {
    namespace organism {
        struct EntityView {
            Location location;
            Color color;
            Status status;
            Velocity velocity;
            Arms arms;
            Body body;
            Ears ears;
            Eyes eyes;
            Legs legs;
            Mouth mouth;
            
            float distance;
            float angle;
        };

        /// <summary>
        /// Represents the sight percept of an organism, containing information about seen entities.
        /// </summary>
        struct SightPercept {
            std::vector<EntityView> visibleEntities;
        };

    }
}
