cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-config)
file(GLOB headers *.h)
file(GLOB sources *.c)

add_library(dmrshark-config ${headers} ${sources})

target_include_directories(dmrshark-config PUBLIC ${CMAKE_SOURCE_DIR} ${GLIB_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(dmrshark-config ${GLIB_LIBRARIES})
target_compile_options(dmrshark-config PUBLIC ${GLIB_CLFAGS_OTHER})