cmake_minimum_required(VERSION 3.21)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES src/*.cpp)

add_executable(PhysicsEngine ${SOURCES})
target_link_libraries(PhysicsEngine ${SDL2_LIBRARIES})
