cmake_minimum_required(VERSION 3.6)
project(amyspeak)

include(compile_flags.cmake)

set(CMAKE_CXX_STANDARD 14)

include_directories(inc)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(amyspeak ${SOURCES})
add_executable(amyspeak_static ${SOURCES})
set_target_properties(amyspeak_static PROPERTIES LINK_FLAGS "-static")