cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-coding)

file(GLOB headers *.h)
file(GLOB sources *.c)

add_library(dmrshark-coding ${headers} ${sources})

target_include_directories(dmrshark-coding PUBLIC ${CMAKE_SOURCE_DIR})