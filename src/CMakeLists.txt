include_directories(
    organism
    world
)

add_executable(SimBio
    main.cpp

    world/systems/Movement.cpp
    world/systems/Death.cpp
    world/systems/FlowerReproduction.cpp
    world/systems/Eating.cpp
    world/systems/Vision.cpp

    organism/organisms/eater/Eater.cpp 
    organism/organisms/mover/Mover.cpp
)

target_compile_definitions(SimBio PRIVATE FLECS_CPP_NO_ENUM_REFLECTION)

target_link_libraries(SimBio
    PRIVATE
        raylib
        flecs
        glm::glm
)

add_custom_command(TARGET SimBio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:flecs>
        $<TARGET_FILE_DIR:SimBio>
)
