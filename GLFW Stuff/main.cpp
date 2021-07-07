#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "objects.h"
#include "shaders.h"
#include "utils.h"

using namespace std;

//Widths and heights in pixels
//You can change the value of these variables and compile it yourself to get different results :)
constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned FFT_SIZE = 512;
constexpr unsigned NUMBER_OF_BARS = 240;
constexpr unsigned SAMPLE_SIZE = 8192;
constexpr int Bar_Width = 3;
constexpr int Bar_Gap = 1;

constexpr float Height_multiplier = 1000;

//The program loops if this is true, else clean everything up and exit
bool Is_playing = true;

const string TITLE = "Visualizer";
const string DEBUG_INFO = "Graphics Libraries: GLFW and GLAD\nAudio processing: Aquila\nAudio: SFML\n";

float Height[NUMBER_OF_BARS];
float PrevHeight[NUMBER_OF_BARS];

void framebuffersize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_inputs(GLFWwindow * _window, sf::Music * _music)
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
		Is_playing = false;
	if (glfwGetKey(_window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (_music->getStatus() == sf::Music::Paused)
			_music->play();
		else
			_music->pause();
	}
}

int main(int argc, char ** argv)
{
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
	Aquila::SignalSource source;
	sf::Music audio;
	if (!audio.openFromFile(argv[1])) {
		cerr << "Error opening music file!" << endl;
		return 2;
	}
	Aquila::WaveFile file(argv[1]);;
	double samples[SAMPLE_SIZE];

	for (size_t i = 0; i < NUMBER_OF_BARS; ++i)
		PrevHeight[i] = 0.f;
	
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
	fstream frag("./shaders/fragment.txt");
	fstream vert("./shaders/vertex.txt");
	shader.loadFragmentShaderSource(frag);
	shader.loadVertexShaderSource(vert);
	shader.compile();
	shader.use();

	Bar bar;
	//Set the window's width and height as the bar's maximum mapping value.
	bar.set_roof(WINDOW_WIDTH, WINDOW_HEIGHT);
	bar.set(0, 0, Bar_Width, 1);

/***************BUFFERS AND STUFF**************/
/**********************************************/
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(0));
	glEnableVertexAttribArray(0);

/**********************************************/

	//Get the uniform value locations
	int indexLocation = glGetUniformLocation(shader.get_program(), "bar_index");
	int widthLocation = glGetUniformLocation(shader.get_program(), "width");
	int heightLocation = glGetUniformLocation(shader.get_program(), "height");
	int timeLocation = glGetUniformLocation(shader.get_program(), "time");
	int totalBarsLocation = glGetUniformLocation(shader.get_program(), "total_bars");
	glUniform1i(widthLocation, WINDOW_WIDTH);
	glUniform1i(heightLocation, WINDOW_HEIGHT);
	glUniform1i(totalBarsLocation, NUMBER_OF_BARS);

	audio.play();

	//Main loop
	while (Is_playing == true && glfwWindowShouldClose(window) == false)
	{
		//If the song has reached its end, close
		if (audio.getPlayingOffset() >= audio.getDuration())
		{
			Is_playing = false;
			glfwSetWindowShouldClose(window, true);
			break;
		}
		process_inputs(window, &audio);

		//Update the samples and calculate the fft to get the spectrum
		copy_samples(file, (((float)audio.getPlayingOffset().asMilliseconds()/1000) * audio.getSampleRate()), samples, SAMPLE_SIZE);
		spectrum = fft.fft(samples);
		fft.ifft(spectrum, samples);
		//Calculate the height
		calculate_height(Height, NUMBER_OF_BARS, spectrum, file.getSampleFrequency(), PrevHeight, FFT_SIZE);
		copy(begin(Height), end(Height), begin(PrevHeight));

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Loops through each bar and changes its properties.
		for (size_t i = 0; i < NUMBER_OF_BARS; ++i)
		{
			//Set the colours
			bar.set(i * (Bar_Width + Bar_Gap), 0, Bar_Width, (float)Height[i] * Height_multiplier);

			//The buffers and uniform values must be updated on every iteration.
			glUniform1i(indexLocation, i);
			glUniform1f(timeLocation, glfwGetTime() * 1000);
			glBufferData(GL_ARRAY_BUFFER, bar.vertices_size(), bar.get_vertices(), GL_DYNAMIC_DRAW);
			bar.draw(window);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.deleteShaders();
	glfwTerminate();
	return 0;
}