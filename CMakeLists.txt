cmake_minimum_required(VERSION 3.20)

project(metal)

set(CMAKE_CXX_STANDARD 20)

add_executable(mel src/main.cpp
        src/parser.hpp
        src/generation.hpp)

