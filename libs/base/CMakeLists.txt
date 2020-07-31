cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-base)

file(GLOB headers *.h)
file(GLOB sources *.c)

add_library(dmrshark-base ${headers} ${sources})

target_include_directories(dmrshark-base PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(dmrshark-base LINK_PUBLIC m)