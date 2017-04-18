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

if (${APPLE})
    SET(GLFW-lib glfw)
    link_directories(third_party/glfw/lib/macos)
elseif(${WIN32})
    SET(GLFW-lib glfw3)
    link_directories(third_party/glfw/lib/win)

    include_directories(third_party/glew/include)
    link_directories(third_party/glew/lib/)
    link_libraries(glew32)
    add_definitions(-DGLEW_STATIC)
endif()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
add_executable(opengl ${SOURCE_FILES})
target_link_libraries(
        opengl
        ${GLFW-lib}
        ${OPENGL_LIBRARY}
)
