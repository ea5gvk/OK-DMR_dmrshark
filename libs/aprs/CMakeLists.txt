cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-aprs)

file(GLOB headers *.h)
file(GLOB sources *.c)

add_library(dmrshark-aprs ${headers} ${sources})

target_include_directories(dmrshark-aprs PUBLIC ${CMAKE_SOURCE_DIR})

