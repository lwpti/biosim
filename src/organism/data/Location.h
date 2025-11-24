#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; describes a 2D position and rotation.
		/// </summary>
		struct Location {
			float x, y, yaw;
			int chunk;
		};
	}
}