cmake_minimum_required(VERSION 3.10.0)
project(oreik_test CXX)

add_executable(oreik_test main.cc)
target_link_libraries(oreik_test oreik)
