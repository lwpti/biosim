#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; allows an organism to perceive sight.
		/// </summary>
		struct Eyes {
			float size;

			constexpr static float MIN_SIZE = 0, MAX_SIZE = 5;			
		};
	}
}