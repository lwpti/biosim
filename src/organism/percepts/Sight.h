#pragma once

#include <flecs.h>
#include "data/Location.h"
#include "organs/Organs.h"
#include "data/Status.h"
#include "data/Color.h"
#include "data/Velocity.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include "Entity.h"

namespace biosim {
    namespace organism {
        struct EntityView {
            Color color { 255, 255, 255 };
            Status status { 0.0f, 0.0f };
            Organs organs;
            Velocity velocity{ 0.0f, 0.0f };
            float distance{ 0.0f };
            float direction{ 0.0f };

            float getSize() const {
                return (std::max)(Organs::sizeOf(organs.body), Organs::sizeOf(organs.flower)) +
                    (std::max)({ Organs::sizeOf(organs.arms),
                        Organs::sizeOf(organs.legs),
                        Organs::sizeOf(organs.mouth) });
            }

            EntityView(const Entity& e, Velocity velocity, float distance, float direction) {
                color = e.color;
                status = e.status;
                organs = e.organs;
                this->velocity = velocity;
                this->distance = distance;
                this->direction = direction;
            }
        };

        struct Sight {
            std::vector<EntityView> visibleEntities;
        };

    }
}
