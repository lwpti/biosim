add_executable(SimBio
        main.cpp
        world/systems/Display.cpp
        world/systems/Movement.cpp
        organism/organisms/mover/Mover.cpp
)

include_directories(
        organism
        world
        ../flecs/include
        ../raylib/src
)
