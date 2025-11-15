#include <mover/Mover.h>

namespace mover {
	using namespace simbio::organism;

	Mover::Mover(flecs::world& world) : Organism<MoverComponent>(world) {}

	flecs::entity Mover::create() const {
		return world.entity()
			.set<Body>({ 10 })
			.set<Legs>({ 10 });
	}

	flecs::entity Mover::create(const flecs::entity& parent) const {
		return world.entity()
			.set<Body>({ 10 })
			.set<Legs>({ 10 });
	}

	flecs::entity Mover::create(const flecs::entity& mother, const flecs::entity& father) const {
		return world.entity()
			.set<Body>({ 10 })
			.set<Legs>({ 10 });
	}

	void Mover::think(const Organs& organs, Intents& intents) const {
	}
}