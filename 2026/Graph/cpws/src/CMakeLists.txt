cmake_minimum_required(VERSION 3.16)
project(cpws CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main src/main.cpp)

target_compile_options(main PRIVATE -O2 -pipe -Wall -Wextra -Wshadow -Wconversion -Wno-sign-conversion)
