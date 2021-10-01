# music-viualizer
A simple music visualizer made in C++

## Compiling

### Windows x64
1) Install CMake and Visual Studio 2019.
2) Clone the repository.
3) Open Command Prompt and cd into the repo folder.
4) Create a build directory using `mkdir build`
5) Cd into the build folder using `cd build`.
6) Execute the following command to configure
     ```cmake -DCMAKE_BUILD_TYPE=Release .. # Release config```
7) Execute the following command to build ```cmake --build . --config Release```
8) Execute the following command to install ```cmake --install . --config Release --prefix <install-directory>```

__NOTE__: Replace the `Release` with `Debug` for the debug config.

[![CMake Debug](https://github.com/dotslashinit-sh/music-visualizer/actions/workflows/cmake-windows-debug.yml/badge.svg)](https://github.com/dotslashinit-sh/music-visualizer/actions/workflows/cmake-windows-debug.yml)

[![CMake Release](https://github.com/dotslashinit-sh/music-visualizer/actions/workflows/cmake-windows-release.yml/badge.svg)](https://github.com/dotslashinit-sh/music-visualizer/actions/workflows/cmake-windows-release.yml)

## Screenshot
![visualizer-gif](images/visualizer-updated.gif)

## Note
- I made this project during my free time. I know this project contains a lot of broken code but I am still learning and I promise to get better.
- MP3 is not supported.

## License
This project is completely free to be used in any way you want without any royalty.
