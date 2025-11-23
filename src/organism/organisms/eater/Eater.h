#pragma once

#include <flecs.h>
#include "Organism.h"

namespace eater {
	using namespace simbio::organism;
	
	struct EaterBrain {};

	class Eater : Organism<EaterBrain> {
	public:
		Eater(flecs::world& world);
		virtual flecs::entity create() const;
		virtual flecs::entity create(const flecs::entity& parent) const;
		virtual flecs::entity create(const flecs::entity& mother, const flecs::entity& father) const;
		virtual void think(const Organs& organs, Intents& intents) const;
	};
}