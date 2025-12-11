include_directories(
    organism
    world
)

add_executable(SimBio
    main.cpp

    world/systems/Movement.cpp
    world/systems/Death.cpp
    world/systems/FlowerReproduction.cpp
    world/systems/Biting.cpp
    world/systems/Hearing.cpp
    world/systems/Vision.cpp
    world/systems/Grabbing.cpp

    world/World.cpp

    organism/organs/Mouth.cpp

    organism/data/Location.cpp
    
    organism/organisms/eater/Eater.cpp 
    organism/organisms/mover/Mover.cpp
)

target_compile_definitions(SimBio PRIVATE FLECS_CPP_NO_ENUM_REFLECTION)

target_link_libraries(SimBio
    PRIVATE
        raylib
        flecs
)

add_custom_command(TARGET SimBio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:flecs>
        $<TARGET_FILE_DIR:SimBio>
)
