#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; describes health and energy of an organism.
		/// </summary>
		struct Status {
			float health, energy;

			constexpr static float MAX_HEALTH = 100.0f;
			constexpr static float MAX_ENERGY = 100000.0f;
			constexpr static float BITE_ENERGY = MAX_ENERGY / MAX_HEALTH;
			constexpr static float FLOWER_ENERGY = MAX_ENERGY / 5.0f;
		};
	}
}