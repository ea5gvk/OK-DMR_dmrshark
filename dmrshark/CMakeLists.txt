cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-bin)

file(GLOB dmrshark_sources "*.c" "*.h")
file(GLOB dmrshark_raws "*.raw")

file(COPY ${dmrshark_raws} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(dmrshark ${dmrshark_sources})

configure_file(defaults.h.in defaults.h)

target_include_directories(dmrshark PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR})
target_link_libraries(dmrshark LINK_PRIVATE dmrshark-config dmrshark-comm dmrshark-base dmrshark-aprs dmrshark-coding dmrshark-daemon dmrshark-dmrpacket dmrshark-remotedb dmrshark-voicestreams)
target_link_libraries(dmrshark LINK_PUBLIC pthread)
