cmake_minimum_required(VERSION 3.28)
project(SimBio LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/raylib)
add_subdirectory(external/flecs)

add_executable(SimBio)

target_sources(SimBio PRIVATE
    src/main.cpp

    src/world/systems/Movement.cpp
    src/world/systems/Death.cpp
    src/world/systems/FlowerReproduction.cpp
    src/world/systems/Biting.cpp
    src/world/systems/Hearing.cpp
    src/world/systems/Vision.cpp
    src/world/systems/Grabbing.cpp

    src/world/World.cpp

    src/organism/organs/Mouth.cpp
    src/organism/organs/Legs.cpp

    src/organism/data/Location.cpp

    src/organism/organisms/eater/Eater.cpp
    src/organism/organisms/mover/Mover.cpp
)

target_include_directories(SimBio PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/organism>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/world>
)

target_link_libraries(SimBio PRIVATE flecs_static raylib)

target_compile_definitions(SimBio PRIVATE FLECS_CPP_NO_ENUM_REFLECTION)