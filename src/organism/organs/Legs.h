#pragma once

#include <cmath>
#include <algorithm>

namespace simbio {
	class World;

	namespace organism {
		/// <summary>
		/// flecs component; Legs size determines speed and will determine success
		/// when an organism is trying to escape another one grabbing it.
		/// </summary>
		struct Legs {
			float size;
			float forwardPower = 1.0f;
			float backwardPower = 1.0f;
			float leftwardPower = 1.0f;
			float rightwardPower = 1.0f;

			constexpr static float MIN_SIZE = 0.0f, MAX_SIZE = 5.0f;
			constexpr static float MAX_OVERSIZE = 1.5f;

			static void registerLegsObserver(World& world);
		};

		/// <summary>
		/// flecs component; describes a desired acceleration direction and magnitude
		/// and angular acceleration, and later also the desire to escape a grab.
		/// </summary>
		struct LegsIntent {
			struct {
				// The desired acceleration componenets relative to current Location.yaw
				float mag, dir;
			} a;
			// The desired rate of yaw in rad/s
			float yaw;
		};

		/// <summary>
		/// flecs component; describes a requested acceleration direction and magnitude
		/// and angular acceleration, and later also the desire to escape a grab.
		/// </summary>
		struct LegsRequest {
			struct {
				float x, y, mag, dir;
			} a;
			float yaw;
			bool valid = false;

			void set(const LegsIntent& intent) {
				yaw = intent.yaw;
				a.x = intent.a.mag * std::cos(intent.a.dir);
				a.y = intent.a.mag * std::sin(intent.a.dir);
				a.mag = intent.a.mag;
				a.dir = intent.a.dir;
			}
		};
	}
}