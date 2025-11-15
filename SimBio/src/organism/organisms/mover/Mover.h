#pragma once

#include <flecs.h>
#include "Organism.h"
#include <vector>

namespace mover {
	using namespace simbio::organism;
	
	struct MoverComponent {
	};

	class Mover : Organism<MoverComponent> {
	public:
		Mover(flecs::world& world);
		virtual flecs::entity create() const;
		virtual flecs::entity create(const flecs::entity& parent) const;
		virtual flecs::entity create(const flecs::entity& mother, const flecs::entity& father) const;
		virtual void think(const Organs& organs, Intents& intents) const;
	};
}