cmake_minimum_required(VERSION 3.7)
project(opengl)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} __gl_h_)

find_package(OpenGL REQUIRED)
include_directories(
        ${OPENGL_INCLUDE_DIR}
        third_party/glfw/include
        third_party/glm
)

link_directories(third_party/glfw/lib/macos)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
add_executable(opengl ${SOURCE_FILES})
target_link_libraries(
        opengl
        glfw
        ${OPENGL_LIBRARY}
)
