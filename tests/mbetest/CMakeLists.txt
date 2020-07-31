cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-test-mbetest)

add_executable(test-mbetest mbetest.c)
target_link_libraries(test-mbetest LINK_PUBLIC m mbe)