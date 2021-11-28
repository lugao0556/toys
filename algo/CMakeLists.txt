# Set the project name
project (algo)

# Add a library with the above sources
add_library(${PROJECT_NAME} src/sort.cpp)
add_library(sub::algolib ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
