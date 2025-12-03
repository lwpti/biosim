#include "Eater.h"
#include <random>
#include <numbers>

namespace eater {
	using namespace simbio::organism;

	Eater::Eater(flecs::world& world) : Organism<EaterBrain>(world) {}

	flecs::entity Eater::create() const {
		return world.entity()
			.add<EaterBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 })
			.set<Mouth>({ 10 })
			.set<simbio::organism::Color>({ 149, 242, 21});
	}

	flecs::entity Eater::create(const flecs::entity& parent) const {
		return world.entity()
			.add<EaterBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 })
			.set<Mouth>({ 10 })
			.set<simbio::organism::Color>({ 149, 242, 21});
	}

	flecs::entity Eater::create(const flecs::entity& mother, const flecs::entity& father) const {
		return world.entity()
			.add<EaterBrain>()
			.set<Body>({ 10})
			.set<Legs>({ 10 })
			.set<Mouth>({ 10 })
			.set<simbio::organism::Color>({ 149, 242, 21});
	}

	// Currently, the Mover brain just accelerates in a random direction.
	void Eater::think(const Percepts& percepts, const Organs& organs, Intents& intents) const {
		float ax = -20.0f + (rand() / (float)RAND_MAX) * 40.0f;
		float ay = -20.0f + (rand() / (float)RAND_MAX) * 40.0f;
		intents.legs = LegsIntent({ ax, ay }, 0.0f);
	}
}
