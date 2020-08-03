cmake_minimum_required(VERSION 3.16.3)
project(dmrshark)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED GLOBAL IMPORTED_TARGET glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

pkg_check_modules(MYSQLCLIENT REQUIRED mysqlclient)
pkg_check_modules(PCAP REQUIRED libpcap)
pkg_check_modules(WEBSOCKETS REQUIRED libwebsockets)

add_subdirectory(libs)
add_subdirectory(dmrshark)
add_subdirectory(tests)
