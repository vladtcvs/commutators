project(commutators)
add_definitions(-Wall)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(commutators ad.cc basic_funcs.cc comm.cc display.cc main.cc tests.cc)
