cmake_minimum_required(VERSION 3.7)
project(opengl)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
include_directories(
        ${OPENGL_INCLUDE_DIR}
        third_party/glfw/include
)

link_directories(third_party/glfw/lib/macos)

set(SOURCE_FILES main.cpp)
add_executable(opengl ${SOURCE_FILES})
target_link_libraries(
        opengl
        glfw
        ${OPENGL_LIBRARY}
)
