#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; body size will have high impact on weight and therefore on
		/// energy usage. It also will determine how much an organism can eat.
		/// </summary>
		struct Body {
			float size;

			constexpr static float MIN_SIZE = 1.0f, MAX_SIZE = 5.0f;
		};
	}
}
