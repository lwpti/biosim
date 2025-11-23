#include <cmath>
#include "Movement.h"
#include "organs/Legs.h"
#include "data/Location.h"
#include "data/Velocity.h"

namespace simbio {
    namespace systems {
        using namespace simbio::organism;

        void Movement::registerMoveIntentSystem(flecs::world& world) {
            world.system<Legs, LegsIntent>("MoveIntentSystem")
                .each([](flecs::entity e, const Legs& legs, const LegsIntent& legsIntent) 
                    {
                        e.set<Move>({ 
                        legsIntent.aDirection, 
                        legsIntent.aMagnitude < legs.size / 3 ? legsIntent.aMagnitude : legs.size / 3, 
                        legsIntent.aAngular });
                        e.remove<LegsIntent>();
                    });
        }

        void Movement::registerMovementSystem(flecs::world& world, int worldWidth, int worldHeight) {
            this->worldWidth = worldWidth;
            this->worldHeight = worldHeight;
            world.system<Move, Location, Velocity>("MovementSystem")
                .each([this](flecs::iter& itr, size_t i, const Move& move, Location& location, Velocity& velocity)
                    {
                        float dt = itr.delta_time();
                        float ax = cos(move.aDirection) * move.aMagnitude;
                        float ay = sin(move.aDirection) * move.aMagnitude;
                        location.x += dt * velocity.x + dt * dt * ax / 2;
                        location.x = location.x > this->worldWidth ? 0 : location.x;
                        location.x = location.x < 0 ? this->worldWidth : location.x;
                        location.y += dt * velocity.y + dt * dt * ay / 2;
                        location.y = location.y > this->worldHeight ? 0 : location.y;
                        location.y = location.y < 0 ? this->worldHeight : location.y;
                        velocity.x += dt * ax;
                        velocity.y += dt * ay;
                        flecs::entity e = itr.entity(i);
                        e.remove<Move>();
                    });
        }
    }
}