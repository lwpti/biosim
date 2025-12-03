#pragma once

#include <flecs.h>
#include "data/Location.h"
#include "organs/Organs.h"
#include "data/Status.h"
#include "data/Color.h"
#include "data/Velocity.h"
#include "Organism.h"
#include <vector>
#include <cmath>
#include "Entity.h"

namespace simbio {
    namespace organism {
        struct EntityView {
            Color color { 255, 255, 255 };
            Location location;
            Status status {0.0f, 0.0f };
            Velocity velocity{ 0.0f, 0.0f };
            Organs organs;

            float getSize() const {
                return (std::max)(organs.body.size, organs.flower.size) +
                       (std::max)({ organs.arms.size, organs.legs.size, organs.mouth.size });
            }

            EntityView(const Entity& e) {
                color = e.color;
                location = e.location;
                status = e.status;
                velocity = e.velocity;
                organs = e.organs;
            }
        };

        struct Sight {
            std::vector<EntityView> visibleEntities;
        };

    }
}
