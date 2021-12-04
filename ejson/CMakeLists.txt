# Set the project name
project (ejson)

# Add a library with the above sources
add_library(${PROJECT_NAME} src/ejson.cpp src/ejson_test.cpp)
add_library(sub::ejsonlib ALIAS ${PROJECT_NAME})


target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)