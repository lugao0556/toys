# Set the project name
project (logger)

# Add a library with the above sources
add_library(${PROJECT_NAME} src/logger.cpp src/logger_test.cpp)
add_library(sub::loggerlib ALIAS ${PROJECT_NAME})


target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
