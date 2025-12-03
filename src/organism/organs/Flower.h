#pragma once

#include "data/Color.h"

namespace simbio {
	namespace organism {
		struct Flower {
			float size;

			constexpr static float MIN_SIZE = 1, MAX_SIZE = 5;
			constexpr static float MIN_REPRODUCTION_TIMER = 90.0f, REPRODUCTION_TIMER_SPREAD = 90.0f;
			// TODO: Adjust MAX_REPRODUCTION_TIME if time step changes.
			constexpr static int MAX_REPRODUCTION_TICKS = 4.0f * (MIN_REPRODUCTION_TIMER + REPRODUCTION_TIMER_SPREAD) / 0.1f;
			constexpr static simbio::organism::Color FLOWER_COLOR{ 0, 228, 48 };
		};
	}
}
