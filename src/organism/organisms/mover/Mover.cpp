#include <organisms/mover/Mover.h>
#include <random>
#include <numbers>

namespace mover {
	using namespace simbio::organism;

	Mover::Mover(flecs::world& world) : Organism<MoverBrain>(world) {}

	flecs::entity Mover::create() const {
		return world.entity()
			.add<MoverBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 });
	}

	flecs::entity Mover::create(const flecs::entity& parent) const {
		return world.entity()
			.add<MoverBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 });
	}

	flecs::entity Mover::create(const flecs::entity& mother, const flecs::entity& father) const {
		return world.entity()
			.add<MoverBrain>()
			.set<Body>({ 10 })
			.set<Legs>({ 10 });
	}

	// Currently, the Mover brain just accelerates in a random direction.
	void Mover::think(const Organs& organs, Intents& intents) const {
		float direction = (rand() / (float)RAND_MAX) * 2.0f * std::numbers::pi_v<float>;
		float magnitude = (rand() / (float)RAND_MAX) * 20.0f;
		intents.legs = LegsIntent(direction, magnitude, 0.0f);
	}
}