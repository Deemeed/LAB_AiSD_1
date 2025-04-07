cmake_minimum_required(VERSION 3.0)
project(lab2 CXX)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_executable(lab2 main.cpp "hashTable.h"  )