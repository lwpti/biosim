#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; body size will have high impact on weight and therefore on
		/// energy usage. It also will determine how much an organism can eat.
		/// </summary>
		struct Body {
			int size;
		};

		/// <summary>
		/// flecs component; there are no actions associated with the Body component so I'll
		/// delete this later.
		/// </summary>
		struct BodyIntent {
		};
	}
}