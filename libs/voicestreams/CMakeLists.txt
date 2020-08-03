cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-voicestreams)

file(GLOB headers *.h)
file(GLOB sources *.c)

add_library(dmrshark-voicestreams ${headers} ${sources})

target_include_directories(dmrshark-voicestreams PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(dmrshark-voicestreams PUBLIC mbe mp3lame)