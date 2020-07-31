cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-comm)

file(GLOB headers *.h)
file(GLOB sources *.c)

add_library(dmrshark-comm ${headers} ${sources})

find_library(NETSNMP "netsnmp")

target_include_directories(dmrshark-comm PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(dmrshark-comm LINK_PUBLIC pcap ${NETSNMP} websockets)