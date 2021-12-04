cmake_minimum_required(VERSION 3.7)

set (CMAKE_CXX_COMPILE "/usr/local/gcc/bin/g++")
add_compile_options(-std=c++11 -Wall -pthread -g)


project(toys)

# Add sub directories
add_subdirectory(src)
add_subdirectory(algo)
add_subdirectory(data_structure)
add_subdirectory(logger)
add_subdirectory(design_patterns)
add_subdirectory(ejson)



