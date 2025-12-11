#pragma once

#include "flecs.h"
#include <optional>
#include <type_traits>
#include "data/Data.h"
#include "organs/Organs.h"
#include "percepts/Percepts.h"
#include "Entity.h"
#include <cmath>

namespace simbio {
	namespace organism {
		// This needs to be verified. Is this enough (or too much) to ensure T will work with flecs?
		template <typename T>
		concept FlecsComponent =
			std::is_trivially_copyable_v<T> &&
			std::is_trivially_destructible_v<T> &&
			std::is_standard_layout_v<T> &&
			!std::is_pointer_v<T> &&
			!std::is_reference_v<T> &&
			!std::is_abstract_v<T>;

		template <FlecsComponent Brain>
		class Organism {
		public:
			virtual ~Organism() = default;

			/// <summary>
			/// Creates a new organism flecs entity from thin air.
			/// </summary>
			/// <returns>The new organism entity</returns>
			virtual flecs::entity create() const = 0;

			/// <summary>
			/// Creates a new organism from a single parent - asexual reproduction.
			/// </summary>
			/// <param name="parent">The parent organism entity</param>
			/// <returns>The child organism entity</returns>
			virtual flecs::entity create(const flecs::entity& parent) const = 0;

			/// <summary>
			/// Creates a new organism from a two parents - sexual reproduction.
			/// </summary>
			/// /// <param name="mother">The first parent organism entity</param>
			/// <param name="father">The second parent organism entity</param>
			/// <returns>The child organism entity</returns>
			virtual flecs::entity create(const flecs::entity& mother, const flecs::entity& father) const = 0;

		protected:
			/// <summary>
			/// Convenient structure for think method
			/// </summary>
			struct Intents {
				std::optional<LegsIntent> legs;
				std::optional<BiteIntent> bite;
			};

			/// <summary>
			/// Given a struct of organs (and later a struct of percepts) decides on what an organism should do and
			/// adds intent flecs components representing what it should do.
			/// </summary>
			virtual void think(Brain& brain, const Status& status, const Percepts& percepts, 
				const Velocity& velocity, const Organs& organs, Intents& intents) const = 0;

			/// <summary>
			/// Registers Brain flecs systems. The behavior of these systems is defined by the think methods of
			/// subclasses of Organism. When an intance of a subclass of Organism is constructed, this constructor
			/// is automatically called, which results in the think method automatically being registered as part of a
			/// flecs system.
			/// </summary>
			/// <param name="world">flecs world which the system is registed in</param>
			Organism(flecs::world& world) : world(world) {
				world.system<Brain, Status, Location, Velocity>()
					.each([this](flecs::entity e, Brain& brain, const Status& status, 
						const Location& location, const Velocity& velocity) {
					Organs organs;
					if (const Arms* arms = e.try_get<Arms>()) organs.arms = *arms;
					if (const Body* body = e.try_get<Body>()) organs.body = *body;
					if (const Ears* ears = e.try_get<Ears>()) organs.ears = *ears;
					if (const Eyes* eyes = e.try_get<Eyes>()) organs.eyes = *eyes;
					if (const Flower* flower = e.try_get<Flower>()) organs.flower = *flower;
					if (const Legs* legs = e.try_get<Legs>()) organs.legs = *legs;
					if (const Mouth* mouth = e.try_get<Mouth>()) organs.mouth = *mouth;

					Percepts percepts;
					if (const Sight* sight = e.try_get<Sight>()) {
						percepts.sight = *sight;
						e.remove<Sight>();
					}
					if (const Sound* sound = e.try_get<Sound>()) {
						percepts.sound = *sound;
						e.remove<Sound>();
					}

					Intents intents;
					
					float yawX = std::cos(location.yaw);
					float yawY = std::sin(location.yaw);
					Velocity relativeV{ 
						velocity.x * yawX + velocity.y * yawY, -velocity.x * yawY + velocity.y * yawX 
					};
					
					this->think(brain, status, percepts, relativeV, organs, intents);

					if (intents.legs.has_value()) {
						if (LegsRequest* request = e.try_get_mut<LegsRequest>()) {
							request->set(*intents.legs);
						} else {
							LegsRequest legsRequest;
							legsRequest.set(*intents.legs);
							e.set<LegsRequest>(legsRequest);
						}
					}
					if (intents.bite.has_value()) e.set<BiteIntent>(*intents.bite);
				});
			}

			flecs::world& world;
		};
	}
}
