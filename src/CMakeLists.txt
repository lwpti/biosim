include_directories(
        organism
        world
)

add_executable(SimBio
    main.cpp

    world/systems/Movement.cpp
    world/systems/Death.cpp

    organism/organisms/eater/Eater.cpp 
    organism/organisms/mover/Mover.cpp
)

target_link_libraries(SimBio raylib flecs)

add_custom_command(TARGET SimBio POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:flecs>
            $<TARGET_FILE_DIR:SimBio>
)
