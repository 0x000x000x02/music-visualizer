# CMakeLists.txt: CMake project to configure and link the GLFW library for the Music Visualizer project.

cmake_minimum_required (VERSION 3.9)

add_library (GLFW INTERFACE)

target_include_directories (GLFW INTERFACE "${CMAKE_CURRENT_LIST_DIR}/Include")
target_link_libraries (GLFW INTERFACE "glfw3")

if (MSVC)
    # Seperate release and debug configurations on windows.
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set (GLFW_LIB_DIR "Windows/Libraries/Debug")
    else ()
        set (GLFW_LIB_DIR "Windows/Libraries/Release")
    endif ()
elseif (UNIX)
    # Unix configuration to be added soon...
endif ()
target_link_directories (GLFW INTERFACE ${GLFW_LIB_DIR})