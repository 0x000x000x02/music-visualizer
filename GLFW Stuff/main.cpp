#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#define VERSION 1.6

#include "objects.h"
#include "shaders.h"
#include "utils.h"

using namespace std;

const unsigned WINDOW_WIDTH = 640;
const unsigned WINDOW_HEIGHT = 480;
const unsigned FFT_SIZE = 1024;
const unsigned NUMBER_OF_BARS = 160;
const unsigned SAMPLE_SIZE = 16384;
int Bar_Width = 3;
int Bar_Height = 10;
int Bar_Gap = 1;

int Height_multiplier = 10;
float Red = 0.f;
float Green = 0.f;
float Blue = 0.f;

//The program loops if this is true, else clean everything up and exit
bool Is_playing = true;

const string TITLE = "Visualizer";
const string DEBUG_INFO = "Graphics Libraries: GLFW and GLAD\nAudio processing: Aquila\nAudio: SFML\n";

float Height[NUMBER_OF_BARS];

void framebuffersize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char ** argv)
{
	cout << "Music visualizer v" << VERSION << endl;

#ifdef _DEBUG
	cout << DEBUG_INFO << endl;
#endif

	if (argc < 2)
	{
		cerr << "Usage: GLFW_Visualizer <music_file_to_play>" << endl;
		glfwTerminate();
		return -1;
	}

	Aquila::AquilaFft fft(FFT_SIZE);
	Aquila::SpectrumType spectrum;
	Aquila::WaveFile file(argv[1]);
	sf::Music audio;
	double samples[SAMPLE_SIZE];

	audio.openFromFile(argv[1]);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE.data(), NULL, NULL);

	//Check for any errors creating the window.
	if (window == NULL)
	{
		cerr << "Error creating window!!" << endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Load GLAD.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cerr << "Error inititalizing GLAD!" << endl;
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Set the window resize callback function.
	glfwSetFramebufferSizeCallback(window, framebuffersize_callback);

	ShaderProgram shader;
	fstream frag("shaders/fragment.txt");
	shader.loadFragmentShaderSource(frag);

	fstream vert("shaders/vertex.txt");
	shader.loadVertexShaderSource(vert);

	shader.compile();

	Bar bar;

	//Set the window's width and height as the bar's maximum mapping value.
	bar.set_roof(WINDOW_WIDTH, WINDOW_HEIGHT);
	bar.set(0, 0, Bar_Width, Bar_Height);

	//Buffers
	unsigned vao, vbo, ebo;

	//Generate and bind Vertex array object.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Generate, bind and configure vertex buffer object.
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bar.vertices_size(), bar.get_vertices(), GL_DYNAMIC_DRAW);

	//Generate, bind and configure element buffer object.
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bar.indices_size(), bar.get_indices(), GL_DYNAMIC_DRAW);

	//Vertex attribute pointer
	/*NOTE: We are keeping only the vertices in the buffer and *
	*not any texture coordinates or colour data.*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(0));
	glEnableVertexAttribArray(0);

	//Use the current shader.
	shader.use();

	//Get the uniform variable color's location in the shader program.
	int color = glGetUniformLocation(shader.get_program(), "color");
	
	audio.play();

	while (Is_playing == true)
	{
		//If the song has reached its end, close it
		if (audio.getPlayingOffset() >= audio.getDuration())
		{
			Is_playing = false;
			glfwSetWindowShouldClose(window, true);

			break;
		}

		if (glfwWindowShouldClose(window) == true)
		{
			Is_playing = false;
			break;
		}

		//Update the samples
		copy_samples(file, (((float)audio.getPlayingOffset().asMilliseconds()/1000) * audio.getSampleRate()), samples, SAMPLE_SIZE);

		//Update the spectrum
		spectrum = fft.fft(samples);

		//Calculate the height
		calculate_height(Height, NUMBER_OF_BARS, spectrum, file.getSampleFrequency());

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Loops through each bar and changes its properties.
		for (size_t i = 0; i < NUMBER_OF_BARS; ++i)
		{
			//Calculate colour values in a unique way(see the set_colour(int, float*, float*, float*) function).
			set_color(((int)((float)glfwGetTime() * 10) % 20), &Red, &Green, &Blue);

			//Gradient effect to the bars
			Green += (float)i / ((float)NUMBER_OF_BARS * 1.5);
			Blue += (float)i / ((float)NUMBER_OF_BARS * 1.5);

			//Set the colours
			bar.set_color(Red, Green, Blue);

			bar.set(i * (Bar_Width + Bar_Gap), 0, Bar_Width, Height[i] * Height_multiplier);

			//The buffers and uniform values must be updated on every iteration.
			glUniform3f(color, bar.get_color_r(), bar.get_color_g(), bar.get_color_b());
			glBufferData(GL_ARRAY_BUFFER, bar.vertices_size(), bar.get_vertices(), GL_DYNAMIC_DRAW);

			bar.draw(window);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		sf::sleep(sf::microseconds(1)); //Reduce ghosting.
	}

	glfwTerminate();

	return 0;
}