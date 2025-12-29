#include "Eater.h"

namespace eater {
	using namespace biosim::organism;

	Eater::Eater(flecs::world& world) : Organism<EaterBrain>(world) {}

	flecs::entity Eater::create() const {
		return world.entity()
			.add<EaterBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 })
			.set<Mouth>({ 10, 10 })
			.set<biosim::organism::Color>({ 149, 242, 21 });
	}

	flecs::entity Eater::create(const flecs::entity& parent) const {
		return world.entity()
			.add<EaterBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 })
			.set<Mouth>({ 10, 10 })
			.set<biosim::organism::Color>({ 149, 242, 21 });
	}

	flecs::entity Eater::create(const flecs::entity& mother, const flecs::entity& father) const {
		return world.entity()
			.add<EaterBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 })
			.set<Mouth>({ 10, 10 })
			.set<biosim::organism::Color>({ 149, 242, 21 });
	}

	// Currently, the Eater brain just tries to bite constantly.
	void Eater::think(EaterBrain& brain, const Status& status, const Percepts& percepts, 
		const Velocity& velocity, const Organs& organs, Intents& intents) const {
		intents.bite = BiteIntent(1.0f);
	}
}
