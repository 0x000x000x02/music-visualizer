# CMakeLists.txt: CMake project to build and configure the Music Visualizer project.

cmake_minimum_required(VERSION 3.9)

project("MusicVisualizer" VERSION 3.1)

# Include CPack to generate installers (for fun).
include(CPack)

# Add the third libraries which will be used.
add_subdirectory("Libraries")

# Generate configuration header.
configure_file("CMake/config.h.in" "config.h")

# Add the main module.
add_subdirectory("Visualizer")