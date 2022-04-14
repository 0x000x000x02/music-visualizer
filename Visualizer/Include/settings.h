#pragma once
#include <cassert>

//Widths and heights in pixels
//You can change the value of these variables and compile it yourself to get different results :)

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned FFT_SIZE = 1024;
constexpr unsigned NUMBER_OF_BARS = 240;
constexpr unsigned SAMPLE_SIZE = 2048;
constexpr int BAR_WIDTH = 3;
constexpr int BAR_GAP = 1;
constexpr float HEIGHT_MULTIPLIER = 10.0f;

// Debug information
const char * TITLE = "Visualizer";
const char* DEBUG_INFO = "Graphics Libraries: GLFW and GLAD\nAudio processing: kissfft\nAudio: SFML\n";