cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-dmrpacket)

file(GLOB headers *.h)
file(GLOB sources *.c)

add_library(dmrshark-dmrpacket ${headers} ${sources})

target_include_directories(dmrshark-dmrpacket PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(dmrshark-dmrpacket LINK_PRIVATE dmrshark-coding)