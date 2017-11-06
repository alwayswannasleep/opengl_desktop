cmake_minimum_required(VERSION 3.7)
project(opengl)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} __gl_h_)

find_package(OpenGL REQUIRED)
include_directories(
        ${OPENGL_INCLUDE_DIR}
        third_party/glfw/include
        third_party/glm_headers
        third_party/glm_headers/glm
        third_party/devil/include
        third_party/assimp/include
        third_party/stb_image
)

if (${APPLE})
    SET(GLFW-lib glfw)
    SET(LIBRARIES_DIRECTORY macos)

    add_definitions(-DOS_APPLE)
elseif(${WIN32})
    SET(LIBRARIES_DIRECTORY win)

    SET(GLFW-lib glfw3)

    include_directories(third_party/glew/include)
    link_directories(third_party/glew/lib/)
    link_libraries(glew32)
    add_definitions(-DGLEW_STATIC -DOS_WINDOWS)
endif()

link_directories(third_party/glfw/lib/${LIBRARIES_DIRECTORY})
link_directories(third_party/assimp/lib/${LIBRARIES_DIRECTORY})

link_libraries(assimpd)

add_library(stb_image third_party/stb_image/stb_image.cpp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
add_executable(opengl ${SOURCE_FILES})
target_link_libraries(
        opengl
        stb_image
        ${GLFW-lib}
        ${OPENGL_LIBRARY}
)
