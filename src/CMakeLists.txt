include_directories(
        organism
        world
)

add_executable(SimBio
        main.cpp
        
        world/systems/Movement.cpp
        organism/organisms/mover/Mover.cpp
 "organism/organs/Arms.h" "organism/organs/Ears.h" "organism/organs/Eyes.h" "organism/percepts/Sound.h" "organism/percepts/Touch.h" "world/systems/Vision.h" "world/systems/Hearing.h" "world/systems/Combat.h")

target_link_libraries(SimBio raylib flecs)

add_custom_command(TARGET SimBio POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:flecs>
            $<TARGET_FILE_DIR:SimBio>
)
