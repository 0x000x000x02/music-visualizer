# CMakeLists.txt: CMake project to configure and link aquila library for the Music Visualizer project.

cmake_minimum_required (VERSION 3.9)

add_library (Aquila INTERFACE)

target_include_directories (Aquila INTERFACE "${CMAKE_CURRENT_LIST_DIR}/Include")

if (MSVC)
    # Seperate debug and release configurations for windows.
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set (Aquila_LIB_DIR "Windows/Libraries/Debug")
        set (Aquila_LIB "Aquila-d.lib")
    else ()
        set (Aquila_LIB_DIR "Windows/Libraries/Release")
        set (Aquila_LIB "Aquila.lib")
    endif ()
elseif (UNIX)
    # Unix configuration to be added soon...
endif ()

target_link_libraries (Aquila INTERFACE ${Aquila_LIB})
target_link_directories (Aquila INTERFACE ${Aquila_LIB_DIR})