# CMakelists.txt: CMake project to configure and build the
# GLAD library for the Music Visualizer project.

cmake_minimum_required(VERSION 3.9)

list(APPEND GLAD_Code_Files
    # Source files.
    "Source/glad.c"

    # Include files.
    "Include/glad/glad.h"
    "Include/KHR/khrplatform.h"
)

add_library(GLAD ${GLAD_Code_Files})

target_include_directories(GLAD PUBLIC "${CMAKE_CURRENT_LIST_DIR}/Include")