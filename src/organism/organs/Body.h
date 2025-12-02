#pragma once

#include <raylib.h>

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; body size will have high impact on weight and therefore on
		/// energy usage. It also will determine how much an organism can eat.
		/// </summary>
		struct Body {
			float size;
		};
	}
}
