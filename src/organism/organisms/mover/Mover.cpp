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
			.set<Mouth>({ 10 })
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
	void Mover::think(MoverBrain& brain, const Status& status, const Percepts& percepts, 
		const Velocity& velocity, const Organs& organs, Intents& intents) const {
		float ax = 1000.0f;
		float ay = 1000.0f;
		float yaw = 1000.0f;
		intents.legs = LegsIntent({ 1000.0f, 1000.0f }, 0.0f);
	}
}
