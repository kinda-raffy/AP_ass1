cmake_minimum_required(VERSION 3.21)
project(ap_ass1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++14 -O")

include_directories(.)

add_executable(ap_ass1
        main.cpp
        milestone4.h
        Node.cpp
        Node.h
        NodeList.cpp
        NodeList.h
        PathPlanner.cpp
        PathPlanner.h
        Types.h)
