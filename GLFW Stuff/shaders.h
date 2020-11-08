#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include <GLAD/glad.h>
#include <glfw/glfw3.h>

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(std::string vertex_fname, std::string fragemnt_fname);

	void loadVertexShaderSource(std::istream&);
	void loadFragmentShaderSource(std::istream&);
	void compile();
	void use();

	int get_program() const
	{
		return shader_program;
	}

	~ShaderProgram();
protected:
	void init();
	bool _compile_(int type);
	void _clean_();

private:
	unsigned vertex_shader = 0;
	unsigned fragment_shader = 0;
	unsigned shader_program = 0;

	std::string vertex_shader_src;
	std::string fragment_shader_src;

	const char* vertex_shader_src_ptr;
	const char* fragment_shader_src_ptr;
};

ShaderProgram::ShaderProgram() : fragment_shader_src_ptr(nullptr), vertex_shader_src_ptr(nullptr)
{
	//Create the shaders
	init();
}

ShaderProgram::ShaderProgram(std::string vertex_fname, std::string fragment_fname)
{
	init();
	std::fstream vertex(vertex_fname), fragment(fragment_fname);

	//Load the shader source codes
	loadVertexShaderSource(vertex);
	loadFragmentShaderSource(fragment);
}

void ShaderProgram::init()
{
	//Create the shaders
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	shader_program = glCreateProgram();
}

void ShaderProgram::loadVertexShaderSource(std::istream& stream)
{
	std::string line;
	std::stringstream src_stream;

	while (std::getline(stream, line))
	{
		src_stream << line << std::endl;
	}

	vertex_shader_src = src_stream.str();
	vertex_shader_src_ptr = vertex_shader_src.c_str();
}

void ShaderProgram::loadFragmentShaderSource(std::istream& stream)
{
	std::string line;
	std::stringstream src_stream;

	while (std::getline(stream, line))
	{
		src_stream << line << std::endl;
	}

	fragment_shader_src = src_stream.str();
	fragment_shader_src_ptr = fragment_shader_src.c_str();
}

void ShaderProgram::use()
{
	glUseProgram(shader_program);
}

bool ShaderProgram::_compile_(int type)
{
	/*NOTE: This function selects the shader to compile using an int.*
	 *If the value is less than 0, then the vertex shader is compiled.*
	*If the value is greater than 0, the fragment shader is compiled.*
	*If the value is 0, then the shaders are attached to the shader *
	*program and then the program is linked.*/

	/*Shader program*/
	if (type == 0)
	{
		//Attach shaders and link program
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);

		//Get the link status and check for errors.
		int status;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
		if (!status)
		{
			char log[512];

			//Get the log and print it to console log.
			glGetProgramInfoLog(shader_program, 512, NULL, log);
			std::cerr << log << std::endl;

			return false;
		}
		return true;
	}

	/*Vertex shader*/
	else if (type < 0)
	{
		glShaderSource(vertex_shader, 1, &vertex_shader_src_ptr, NULL);
		glCompileShader(vertex_shader);

		//Get compile status and check for errors.
		int status;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			char log[512];

			//Get the compile log and print it to console log.
			glGetShaderInfoLog(vertex_shader, 512, NULL, log);
			std::cerr << log << std::endl;

			return false;
		}
		return true;
	}

	/*Fragment shader*/
	else if (type > 0)
	{
		glShaderSource(fragment_shader, 1, &fragment_shader_src_ptr, NULL);
		glCompileShader(fragment_shader);

		//Get compile status and check for errors.
		int status;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			char log[512];

			//Get the log and print it to console output.
			glGetShaderInfoLog(vertex_shader, 512, NULL, log);
			std::cerr << log << std::endl;

			return false;
		}
		return true;
	}

	return true;
}

void ShaderProgram::compile()
{
	_compile_(-1);
	_compile_(1);
	_compile_(0);
}

void ShaderProgram::_clean_()
{
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

ShaderProgram::~ShaderProgram()
{
	_clean_();
	glDeleteProgram(shader_program);
}
