# Set the project name
project (data_structure)

# Add a library with the above sources
add_library(${PROJECT_NAME} src/bst.cpp src/linklist.cpp)
add_library(sub::data_structurelib ALIAS ${PROJECT_NAME})


target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
