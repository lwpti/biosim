#pragma once

#include <type_traits>
#include <optional>
#include <flecs.h>
#include "Organs.h"
#include "Location.h"

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

			virtual flecs::entity create() const = 0;
			virtual flecs::entity create(const flecs::entity& parent) const = 0;
			virtual flecs::entity create(const flecs::entity& mother, const flecs::entity& father) const = 0;
			virtual void think(const Organs& organs, Intents& intents) const = 0;	// Percepts should be parameter too

		protected:
			Organism(flecs::world& world) : world(world) {
				world.system<Brain>()
					.each([this](flecs::entity e, const Brain& brain)
						{
							Organs organs;
							organs.body = e.try_get<Body>();
							organs.legs = e.try_get<Legs>();

							Intents intents;

							this->think(organs, intents);
						});
			}

			flecs::world& world;
		};
	}
}