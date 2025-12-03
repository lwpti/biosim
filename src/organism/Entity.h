#pragma once

#include <flecs.h>
#include "data/Location.h"
#include "organs/Organs.h"
#include "data/Status.h"
#include "data/Color.h"
#include "data/Velocity.h"

namespace simbio {
	namespace organism {
        struct Entity {
			flecs::entity_t flecsID;
			Color color { 255, 255, 255 };
			Location location;
			Status status {0.0f, 0.0f };
			Velocity velocity{ 0.0f, 0.0f };
			Organs organs;

			float getSize() const {
				return (std::max)(organs.body.size, organs.flower.size) + (std::max)({ organs.arms.size, organs.legs.size, organs.mouth.size });
			}
		};
    }
}