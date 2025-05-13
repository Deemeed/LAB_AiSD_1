cmake_minimum_required(VERSION 3.12)
project(lab3-demo CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_executable(lab3-demo main.cpp "Graph.h" "HashTable.h")