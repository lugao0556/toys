cmake_minimum_required(VERSION 3.7)

set (CMAKE_CXX_COMPILE "/usr/local/gcc/bin/g++")
##add_compile_options(-std=c++11 -Wall -Werror)
add_compile_options(-std=c++11 -g)

project(algo)



list(APPEND _sources dp.cpp bst.cpp linklist.cpp)

add_executable(algo main.cpp ${_sources})
