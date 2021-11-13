# CMakeLists.txt: CMake project to configure and link the SFML library.

cmake_minimum_required(VERSION 3.9)

add_library(SFML INTERFACE)
target_include_directories (SFML INTERFACE "${CMAKE_CURRENT_LIST_DIR}/Include")

if (MSVC)
    set(SFML_LIB_DIR "Windows/Libraries")

    # Seperate debug and release configuration for windows.
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        list (APPEND SFML_LIBS
            "sfml-audio-d"
            "sfml-main-d"
            "sfml-system-d"
        )

        # Install configuration
        install(FILES 
            "Windows/Binaries/openal32.dll"
            "Windows/Binaries/sfml-audio-d-2.dll"
            "Windows/Binaries/sfml-system-d-2.dll"
            DESTINATION "Visualizer"
        )
    else ()
        list (APPEND SFML_LIBS
            "sfml-audio"
            "sfml-main"
            "sfml-system"
        )

        # Install configuration
        install(FILES 
            "Windows/Binaries/openal32.dll"
            "Windows/Binaries/sfml-audio-2.dll"
            "Windows/Binaries/sfml-system-2.dll"
            DESTINATION "Visualizer"
        )
    endif ()

    # Libraries to use regardless of build type.
    list (APPEND SFML_LIBS
        "flac"
        "freetype"
        "ogg"
        "openal32"
        "vorbis"
        "vorbisfile"
        "vorbisenc"
    )
elseif (UNIX)
    # UNIX libraries to be added...
endif ()

target_link_directories (SFML INTERFACE ${SFML_LIB_DIR})
target_link_libraries (SFML INTERFACE ${SFML_LIBS})