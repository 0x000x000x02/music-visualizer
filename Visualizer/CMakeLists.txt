# CMakeLists.txt: CMake project to build and configure the visualizer module.

cmake_minimum_required(VERSION 3.9)

list(APPEND Visualizer_Code_Files
    #Source Files.
    "Source/main.cpp"

    #Include files.
    "Include/objects.h"
    "Include/shaders.h"
    "Include/utils.h"
)

add_executable(Visualizer ${Visualizer_Code_Files})
target_link_libraries(Visualizer PRIVATE GLFW SFML Aquila GLAD)
target_include_directories(Visualizer PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Include")

# Set install target.
install(TARGETS Visualizer DESTINATION "Binaries")

# Install configuration for the default shader files.
install (FILES
    "Shaders/fragment.txt"
    "Shaders/vertex.txt"
    DESTINATION "Binaries/Shaders")