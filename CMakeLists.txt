# Project: 
cmake_minimum_required(VERSION 3.22)
project(laser)

set(CMAKE_CXX_STANDARD 17)

# C++ headers and source files
file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE SOURCES src/*.c src/*.cpp src/*.hpp)

set(CLI_TARGET laser_client)

add_executable(${CLI_TARGET} ${HEADERS} ${SOURCES})

target_include_directories(${CLI_TARGET} PUBLIC include src)
target_link_directories(${CLI_TARGET} PUBLIC lib)
target_link_libraries(${CLI_TARGET} PRIVATE glfw3 supercell-flash-test-tool)
