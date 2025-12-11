#pragma once

#include <flecs.h>
#include "data/Data.h"
#include "organs/Organs.h"

namespace simbio {
	namespace organism {
        struct Entity {
			flecs::entity_t flecsID;
			Color color;
			Location location;
			Status status {0.0f, 0.0f };
			Velocity velocity{ 0.0f, 0.0f };
			Organs organs;

			float getSize() const {
				return (std::max)(organs.body.size, organs.flower.size);
			}
		};
    }
}