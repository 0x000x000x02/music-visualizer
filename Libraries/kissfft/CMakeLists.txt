# CMakeLists.txt: CMake project to configure the kissfft library.

cmake_minimum_required(VERSION 3.9)

add_library(kissfft INTERFACE)
target_include_directories(kissfft INTERFACE "Include")

# Windows
if(MSVC)
    list(APPEND kissfft_LIB_DIR
        "${CMAKE_CURRENT_LIST_DIR}/Windows/Libraries"
    )
endif()

list(APPEND kissfft_LIBS
    "kissfft-int16_t"
)

target_link_directories(kissfft INTERFACE ${kissfft_LIB_DIR})
target_link_libraries(kissfft INTERFACE ${kissfft_LIBS})