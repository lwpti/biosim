#pragma once

#include <raylib.h>

namespace simbio {
	namespace plants {
		struct Flower {
			int size;
			Color color;
			int reproductionTimer;

			constexpr static int MIN_SIZE = 1, MAX_SIZE = 5;
		};
	}
}
