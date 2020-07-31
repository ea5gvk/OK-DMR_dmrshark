cmake_minimum_required(VERSION 3.16.3)
project(dmrshark-test-gps)

add_executable(test-hytera-gps-but hytera-gps-but.c)
target_link_libraries(test-hytera-gps-but LINK_PUBLIC m)

add_executable(test-hytera-gps-trig hytera-gps-trig.c)
target_link_libraries(test-hytera-gps-trig LINK_PUBLIC m)