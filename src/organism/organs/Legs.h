#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; Legs size determines speed and will determine success
		/// when an organism is trying to escape another one grabbing it.
		/// </summary>
		struct Legs {
			float size;

			constexpr static int MIN_SIZE = 0, MAX_SIZE = 6;

			static void registerLegsObserver(flecs::world& world) {
				world.observer<Legs>("LegsObserver")
					.event(flecs::OnSet)
					.each([](flecs::entity e, Legs& legs) 
						{
							if (legs.size < MIN_SIZE) {
								legs.size = MIN_SIZE;
							}
							else if (legs.size > MAX_SIZE) {
								legs.size = MAX_SIZE;
							}
						});
			}
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
	}
}