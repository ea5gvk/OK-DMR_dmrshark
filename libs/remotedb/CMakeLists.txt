cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-remotedb)

file(GLOB headers *.h)
file(GLOB sources *.c)

add_library(dmrshark-remotedb ${headers} ${sources})

target_include_directories(dmrshark-remotedb PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(dmrshark-remotedb LINK_PUBLIC mysqlclient)