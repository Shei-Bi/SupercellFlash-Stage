# Project: 
cmake_minimum_required(VERSION 3.22)
project(laser)

set(CMAKE_CXX_STANDARD 20)

# C++ headers and source files
file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE SOURCES src/*.c src/*.cpp src/*.hpp)

set(CLI_TARGET laser_client)

add_executable(${CLI_TARGET} ${HEADERS} ${SOURCES})

target_include_directories(${CLI_TARGET} PUBLIC include src)
target_link_directories(${CLI_TARGET} PUBLIC lib)
target_link_libraries(${CLI_TARGET} PRIVATE glfw3 supercell-flash-test-tool libzstd vld)
target_compile_options(${CLI_TARGET} PRIVATE "$<$<CONFIG:Release>:/Zi>")
target_link_options(${CLI_TARGET} PRIVATE "$<$<CONFIG:Release>:/DEBUG>")
target_link_options(${CLI_TARGET} PRIVATE "$<$<CONFIG:Release>:/OPT:REF>")
target_link_options(${CLI_TARGET} PRIVATE "$<$<CONFIG:Release>:/OPT:ICF>")
target_compile_options(${CLI_TARGET} PRIVATE "$<$<CONFIG:Release>:/Ob3>")
target_compile_options(${CLI_TARGET} PRIVATE "$<$<CONFIG:Release>:/GS->")
# target_compile_options(${CLI_TARGET} PRIVATE "-fpermissive" "-g" "-O3")

# windows 并行编译选项
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/Gm->) #高版本已被废弃，但是低版本的Gm会影响并行
cmake_host_system_information(RESULT CPU_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP${CPU_NUMBER_OF_LOGICAL_CORES}>)

add_compile_definitions(NOMINMAX)
add_compile_definitions(MOVIECLIP_DEBUG)