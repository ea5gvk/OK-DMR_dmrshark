cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-daemon)

file(GLOB headers *.h)
file(GLOB sources *.c)

add_library(dmrshark-daemon ${headers} ${sources})

target_include_directories(dmrshark-daemon PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(dmrshark-daemon LINK_PRIVATE dmrshark-base)