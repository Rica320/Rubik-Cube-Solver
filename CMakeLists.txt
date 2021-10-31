cmake_minimum_required(VERSION 3.6)
project(Rubik-Cube-Solver)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/GLFW/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/GLFW/lib-vc2019)

include_directories(lib/MAT-Functions)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/GLEW/include/GL)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/GLEW/lib/Release/Win32)

set(SOURCE_FILES main.cpp GameEngine.cpp GameEngine.h RubikCube.cpp RubikCube.h lib/MAT-Functions/Mat.h lib/MAT-Functions/Mat.cpp lib/Camera/camera.cpp lib/Camera/camera.h)
add_executable(Rubik-Cube-Solver ${SOURCE_FILES})
target_link_libraries(Rubik-Cube-Solver glfw3.lib glew32.lib ${OPENGL_LIBRARIES})