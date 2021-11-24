# Set the project name
project (pc)
add_compile_options(-std=c++11 -pthread -g)

# Add a library with the above sources
add_library(${PROJECT_NAME} src/producer_consumer_pattern.cpp)
add_library(sub::pclib ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)