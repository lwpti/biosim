#pragma once

#include <flecs.h>
#include "Organism.h"

namespace eater {
	using namespace simbio::organism;
	
	struct EaterBrain {
		int dummy;
	};

	class Eater : public Organism<EaterBrain> {
	public:
		Eater(flecs::world& world);
		virtual flecs::entity create() const override;
		virtual flecs::entity create(const flecs::entity& parent) const override;
		virtual flecs::entity create(const flecs::entity& mother, const flecs::entity& father) const override;
		virtual void think(EaterBrain& brain, const Status& status, const Percepts& percepts, 
			const Velocity& velocity, const Organs& organs, Intents& intents) const override;
	};
}