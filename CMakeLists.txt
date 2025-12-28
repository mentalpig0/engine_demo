cmake_minimum_required(VERSION 3.10)
project(OpenGL)

set(CMAKE_CXX_STANDARD 17)

# Find dependencies
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)

# Source files
set(SOURCES
    src/main.cpp
    src/glad.c
    src/core/camera.cpp
    src/core/shader.cpp
    renderer/renderer.cpp
)

# Create executable
add_executable(main ${SOURCES})

# Link libraries
target_link_libraries(main glfw OpenGL::GL)

# Copy shaders to build directory
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})