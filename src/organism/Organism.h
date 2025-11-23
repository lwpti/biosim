#pragma once

#include <flecs.h>
#include <optional>
#include <type_traits>
#include "data/Location.h"
#include "organs/Organs.h"

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
			struct Organs {
				std::optional<Body> body;
				std::optional<Legs> legs;
				std::optional<Mouth> mouth;
			};
			
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
			/// <param name="organs">All the organ components from the organism flecs entity</param>
			/// <param name="intents">Intent components to be added to the flecs entity</param>
			virtual void think(const Organs& organs, Intents& intents) const = 0;	// Percepts should be parameter too

			/// <summary>
			/// Registers Brain flecs systems. The behavior of these systems is defined by the think methods of
			/// subclasses of Organism. When an intance of a subclass of Organism is constructed, this constructor
			/// is automatically called, which results in the think method automatically being registered as part of a
			/// flecs system.
			/// </summary>
			/// <param name="world">flecs world which the system is registed in</param>
			Organism(flecs::world& world) : world(world) {
				world.system<Brain>()
					.each([this](flecs::entity e, const Brain& brain) 
						{
							Organs organs;
							if (const Body* body = e.try_get<Body>()) organs.body = *body;
							if (const Legs* legs = e.try_get<Legs>()) organs.legs = *legs;
							if (const Mouth* mouth = e.try_get<Mouth>()) organs.mouth = *mouth;

							Intents intents;
							
							this->think(organs, intents);
							if (intents.legs.has_value()) e.set<LegsIntent>(*intents.legs);
							if (intents.bite.has_value()) e.set<BiteIntent>(*intents.bite);
						});
			}

			flecs::world& world;
		};
	}
}