# CMakeLists.txt: CMake Project to configure all the include directories and targets required for the music visualizer.

cmake_minimum_required(VERSION 3.9)

# SFML.
add_subdirectory("SFML")

# Aquila.
add_subdirectory("Aquila")

# GLFW.
add_subdirectory("GLFW")

# GLAD.
add_subdirectory("GLAD")