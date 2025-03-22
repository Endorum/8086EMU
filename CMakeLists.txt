cmake_minimum_required(VERSION 3.28)
project(8086EMU)

set(CMAKE_CXX_STANDARD 17)

add_executable(8086EMU src/main.cpp
        include/Tables.hpp
        src/decoder.cpp
        include/decoder.hpp
        src/memory.cpp
        include/memory.hpp)
