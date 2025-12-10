#pragma once
#include <cmath>

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; Legs size determines speed and will determine success
		/// when an organism is trying to escape another one grabbing it.
		/// </summary>
		struct Legs {
			float size;
			
			constexpr static float MIN_SIZE = 0, MAX_SIZE = 5;
		};

		/// <summary>
		/// flecs component; describes a desired acceleration direction and magnitude
		/// and angular acceleration, and later also the desire to escape a grab.
		/// </summary>
		struct LegsIntent {
			struct {
				float x, y;
			} a;
			float yaw;
		};

		/// <summary>
		/// flecs component; describes a requested acceleration direction and magnitude
		/// and angular acceleration, and later also the desire to escape a grab.
		/// </summary>
		struct LegsRequest {
			struct {
				float x, y, magnitude;
			} a;
			float yaw;
			bool valid = false;

			LegsRequest() = default;

			LegsRequest(const LegsIntent& intent)
				: yaw(intent.yaw) {
				a.x = intent.a.x;
				a.y = intent.a.y;
				a.magnitude = std::sqrt(a.x * a.x + a.y * a.y);
			}

			void set(const LegsIntent& intent) {
				yaw = intent.yaw;
				a.x = intent.a.x;
				a.y = intent.a.y;
				a.magnitude = std::sqrt(a.x * a.x + a.y * a.y);
			}
		};
	}
}