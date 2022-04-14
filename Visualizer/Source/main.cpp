#include <config.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <algorithm>

#include <settings.h>
#include <objects.h>
#include <shaders.h>
#include <utils.h>

using namespace std;

//The program loops if this is true, else clean everything up and exit
bool Is_playing = true;

void framebuffersize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_inputs(GLFWwindow * _window, sf::Sound * _sound)
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
		Is_playing = false;
	if (glfwGetKey(_window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (_sound->getStatus() == sf::Sound::Paused)
			_sound->play();
		else
			_sound->pause();
	}
}

int main(int argc, char ** argv)
{
	std::cout << "Music Visualizer version " << PROJECT_VERSION_MAJOR << '.'
			  << PROJECT_VERSION_MINOR << std::endl;
#ifdef _DEBUG
	cout << DEBUG_INFO << endl;
#endif
	assert(((FFT_SIZE / 2) > NUMBER_OF_BARS));

	string filename;
	if (argc < 2)
	{
		cout << "Enter a valid music file: " << endl;
		getline(cin, filename);

		// Remove any quotation marks in the input.
		for (size_t i = 0; i < filename.length(); ++i)
			if (filename[i] == '"')
				filename.erase(i, 1);
	}
	else
		filename = argv[1];

	SpctType frame[SAMPLE_SIZE];
	double Height[NUMBER_OF_BARS];
	SpctType spectrum[FFT_SIZE];
	kissfft<int16_t> fft(FFT_SIZE, false);

	sf::SoundBuffer audioBuffer;
	if(!audioBuffer.loadFromFile(filename))
	{
		std::cout << "Error opening audio file!" << std::endl;
		return EXIT_FAILURE;
	}
	sf::Sound audio(audioBuffer);
	for (size_t i = 0; i < NUMBER_OF_BARS; ++i)
		Height[i] = 0.f;
	
	// Initialize and setup OpenGL.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSwapInterval(60);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE, NULL, NULL);
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
	glfwSetFramebufferSizeCallback(window, framebuffersize_callback);

	ShaderProgram shader;
	fstream frag("./shaders/fragment.txt");
	fstream vert("./shaders/vertex.txt");
	shader.loadFragmentShaderSource(frag);
	shader.loadVertexShaderSource(vert);
	shader.compile();
	shader.use();

	/*
		A single bar object is used throught the program. For each bar, the positions and the height are
		changed and then drawn again without clearing the buffer. This method was adopted to maintain a good
		resource usage.
	*/
	Bar bar;
	// Set the window's width and height as the bar's maximum mapping value.
	bar.set_roof(WINDOW_WIDTH, WINDOW_HEIGHT);
	bar.set(0, 0, BAR_WIDTH, 1);

/***************BUFFERS AND STUFF**************/
/**********************************************/
	unsigned vao, vbo, ebo;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bar.vertices_size(), bar.get_vertices(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bar.indices_size(), bar.get_indices(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(0));
	glEnableVertexAttribArray(0);

/**********************************************/

	//Uniform values.
	int indexLocation = glGetUniformLocation(shader.get_program(), "bar_index");
	int widthLocation = glGetUniformLocation(shader.get_program(), "width");
	int heightLocation = glGetUniformLocation(shader.get_program(), "height");
	int timeLocation = glGetUniformLocation(shader.get_program(), "time");
	int totalBarsLocation = glGetUniformLocation(shader.get_program(), "total_bars");
	glUniform1i(widthLocation, WINDOW_WIDTH);
	glUniform1i(heightLocation, WINDOW_HEIGHT);
	glUniform1i(totalBarsLocation, NUMBER_OF_BARS);

	audio.play();
	int offset = 0;
	const int bufferSize = audioBuffer.getSampleCount();
	const float binSizePerBar = ((float)audioBuffer.getSampleRate()/FFT_SIZE * (NUMBER_OF_BARS - 1))/NUMBER_OF_BARS;

	//Main loop
	while (Is_playing == true && glfwWindowShouldClose(window) == false)
	{
		//If the song has reached its end, close
		if (audio.getPlayingOffset() >= audioBuffer.getDuration())
		{
			Is_playing = false;
			glfwSetWindowShouldClose(window, true);
			break;
		}
		process_inputs(window, &audio);

		//Update the samples and calculate the fft to get the spectrum
		offset = double(audio.getPlayingOffset().asMilliseconds())/1000 * audioBuffer.getSampleRate();

		std::ios::sync_with_stdio(false);
		std::cout << std::setprecision(3);

		copySamples(offset, audioBuffer.getSamples(), bufferSize, frame);
		fft.transform(frame, spectrum);
		//Calculate the height
		calculateHeight(Height, spectrum, audioBuffer.getSampleRate(), binSizePerBar);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Loops through each bar and changes its properties.
		for (size_t i = 0; i < NUMBER_OF_BARS; ++i)
		{
			//Set the colours
			bar.set(i * (BAR_WIDTH + BAR_GAP), 0, BAR_WIDTH, Height[i] * HEIGHT_MULTIPLIER);

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
