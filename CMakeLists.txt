cmake_minimum_required(VERSION 3.28)
project(SimBio)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RAYLIB_VERSION 5.5)

add_compile_definitions(FLECS_CPP_NO_ENUM_REFLECTION)

include(FetchContent)

find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    message(STATUS "raylib ${RAYLIB_VERSION} not found - fetching with FetchContent")

    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )

    FetchContent_MakeAvailable(raylib)
endif()

FetchContent_Declare(
    flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
    GIT_TAG v4.1.2
)
FetchContent_MakeAvailable(flecs)

find_package(glm CONFIG QUIET)

if (NOT glm_FOUND)
    message(STATUS "glm not found - fetching with FetchContent")

    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
    )
    FetchContent_MakeAvailable(glm)
endif()

add_subdirectory(src)
