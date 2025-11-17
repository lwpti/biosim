cmake_minimum_required(VERSION 3.28)
project(SimBio)

add_subdirectory(src)

include_directories(src/organism)
target_link_libraries(SimBio ${SIMBIO_LIBS})
