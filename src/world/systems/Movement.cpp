#include <cmath>
#include "Movement.h"
#include "organs/Legs.h"
#include "data/Location.h"
#include "data/Velocity.h"

namespace simbio {
    namespace systems {
        using namespace simbio::organism;

        struct Move {
            float direction;
            float magnitude;
            float turn;
        };

        void Movement::registerMoveIntentSystem(flecs::world& world) {
            world.system<Legs, LegsIntent>("MoveIntentSystem")
                .each([](flecs::entity e, const Legs& legs, const LegsIntent& legsIntent) 
                    {
                        e.set<Move>({ 
                        legsIntent.direction, 
                        legsIntent.magnitude < legs.size / 3 ? legsIntent.magnitude : legs.size / 3, 
                        legsIntent.turn });
                        e.remove<LegsIntent>();
                    });
        }

        void Movement::registerMovementSystem(flecs::world& world) {
            world.system<Move, Location, Velocity>("MovementSystem")
                .each([](flecs::iter& itr, size_t i, const Move& move, Location& location, Velocity& velocity)
                    {
                        float dt = itr.delta_time();
                        float ax = cos(move.direction) * move.magnitude;
                        float ay = sin(move.direction) * move.magnitude;
                        location.x += dt * velocity.x + dt * ax * ax / 2;
                        location.y += dt * velocity.y + dt * ay * ay / 2;
                        velocity.x += dt * ax;
                        velocity.y += dt * ay;
                        flecs::entity e = itr.entity(i);
                        e.remove<Move>();
                    });
        }
    }
}