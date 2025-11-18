#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; describes velocity broken down into x and y components.
		/// </summary>
		struct Velocity {
			float x;
			float y;
		};
	}
}