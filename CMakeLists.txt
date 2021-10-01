# CMakeLists.txt: CMake project to build and configure the Music Visualizer project.

cmake_minimum_required(VERSION 3.9)

project("Music Visualizer")

# Add the third libraries which will be used.
add_subdirectory("Libraries")

# Add the main module.
add_subdirectory("Visualizer")