cmake_minimum_required(VERSION 3.16)
project(3353_PA_03)

set(CMAKE_CXX_STANDARD 14)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS)



add_executable(3353_PA_03
        football.graphml
        main.cpp)
