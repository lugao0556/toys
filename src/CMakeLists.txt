set (CMAKE_CXX_COMPILE "/usr/local/gcc/bin/g++")
add_compile_options(-std=c++11 -g)


project(demo)

add_executable(${PROJECT_NAME} main.cpp)

# Link the static library from subproject1 using it's alias sub::lib1
# Link the header only library from subproject2 using it's alias sub::lib2
# This will cause the include directories for that target to be added to this project
target_link_libraries(${PROJECT_NAME}
    sub::data_structurelib
    sub::algolib
)