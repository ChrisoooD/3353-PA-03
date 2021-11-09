cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
project(3353_PA_03)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Boost 1.40 COMPONENTS locale graph REQUIRED)

add_executable(3353_PA_03
        football.graphml
        main.cpp)

target_link_libraries(3353_PA_03 Boost::locale Boost::graph)


