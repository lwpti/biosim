#pragma once

#include <flecs.h>
#include "Organism.h"

namespace mover {
	using namespace simbio::organism;
	
	struct MoverBrain {};

	class Mover : public Organism<MoverBrain> {
	public:
		Mover(flecs::world& world);
		virtual flecs::entity create() const;
		virtual flecs::entity create(const flecs::entity& parent) const;
		virtual flecs::entity create(const flecs::entity& mother, const flecs::entity& father) const;
		virtual void think(const Percepts& percepts, const Organs& organs, Intents& intents) const;
	};
}