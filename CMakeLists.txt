cmake_minimum_required(VERSION 3.16)
project(3353_PA_03)

set(CMAKE_CXX_STANDARD 14)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

add_executable(3353_PA_03
        football.graphml
        main.cpp)

target_link_libraries(3353_PA_03 Boost::program_options)


