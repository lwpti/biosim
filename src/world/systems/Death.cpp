#include "Death.h"
#include <data/Status.h>

namespace simbio {
    namespace systems {
        using namespace organism;

        void Death::registerDeathSystem(flecs::world& world)
        {
            world.system<Status>("DeathSystem")
                .each([](flecs::entity e, const Status& status)
                    {
                        if (status.energy <= 0.0f || status.health <= 0.0f) e.destruct();
                    });
        }
    }
}