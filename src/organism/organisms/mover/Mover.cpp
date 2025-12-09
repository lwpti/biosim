    #include "Mover.h"
#include <random>
#include <numbers>

namespace mover {
	Mover::Mover(flecs::world& world) : Organism<MoverBrain>(world) {}

	flecs::entity Mover::create() const {

		return world.entity()
			.add<MoverBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 })
			.set<simbio::organism::Color>({ 41, 242, 141});
	}

	flecs::entity Mover::create(const flecs::entity& parent) const {

		return world.entity()
			.add<MoverBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 })
			.set<simbio::organism::Color>({ 41, 242, 141});
	}

	flecs::entity Mover::create(const flecs::entity& mother, const flecs::entity& father) const {

		return world.entity()
			.add<MoverBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 })
			.set<simbio::organism::Color>({ 41, 242, 141});
	}

	// Currently, the Mover brain just accelerates in a random direction.
	void Mover::think(MoverBrain& brain,  const Status& status, const Percepts& percepts, 
		const Organs& organs, Intents& intents) const {
		float ax = -20.0f + (rand() / (float)RAND_MAX) * 40.0f;
		float ay = -20.0f + (rand() / (float)RAND_MAX) * 40.0f;
		intents.legs = LegsIntent({ ax, ay }, 0.0f);
	}
}
