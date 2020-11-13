#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

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
		return m_shader_program;
	}

	~ShaderProgram();
protected:
	void init();
	bool _compile_(int type);
	void _clean_();

private:
	unsigned m_vertex_shader = 0;
	unsigned m_fragment_shader = 0;
	unsigned m_shader_program = 0;

	std::string m_vertex_shader_src;
	std::string m_fragment_shader_src;

	const char* m_vertex_shader_src_ptr;
	const char* m_fragment_shader_src_ptr;
};

ShaderProgram::ShaderProgram() : m_fragment_shader_src_ptr(nullptr), m_vertex_shader_src_ptr(nullptr)
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
	m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	m_shader_program = glCreateProgram();
}

void ShaderProgram::loadVertexShaderSource(std::istream& stream)
{
	std::string line;
	std::stringstream src_stream;

	while (std::getline(stream, line))
	{
		src_stream << line << std::endl;
	}

	m_vertex_shader_src = src_stream.str();
	m_vertex_shader_src_ptr = m_vertex_shader_src.c_str();
}

void ShaderProgram::loadFragmentShaderSource(std::istream& stream)
{
	std::string line;
	std::stringstream src_stream;

	while (std::getline(stream, line))
	{
		src_stream << line << std::endl;
	}

	m_fragment_shader_src = src_stream.str();
	m_fragment_shader_src_ptr = m_fragment_shader_src.c_str();
}

void ShaderProgram::use()
{
	glUseProgram(m_shader_program);
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
		glAttachShader(m_shader_program, m_vertex_shader);
		glAttachShader(m_shader_program, m_fragment_shader);
		glLinkProgram(m_shader_program);

		//Get the link status and check for errors.
		int status;
		glGetProgramiv(m_shader_program, GL_LINK_STATUS, &status);
		if (!status)
		{
			char log[512];

			//Get the log and print it to console log.
			glGetProgramInfoLog(m_shader_program, 512, NULL, log);
			std::cerr << log << std::endl;

			return false;
		}
		return true;
	}

	/*Vertex shader*/
	else if (type < 0)
	{
		glShaderSource(m_vertex_shader, 1, &m_vertex_shader_src_ptr, NULL);
		glCompileShader(m_vertex_shader);

		//Get compile status and check for errors.
		int status;
		glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			char log[512];

			//Get the compile log and print it to console log.
			glGetShaderInfoLog(m_vertex_shader, 512, NULL, log);
			std::cerr << log << std::endl;

			return false;
		}
		return true;
	}

	/*Fragment shader*/
	else if (type > 0)
	{
		glShaderSource(m_fragment_shader, 1, &m_fragment_shader_src_ptr, NULL);
		glCompileShader(m_fragment_shader);

		//Get compile status and check for errors.
		int status;
		glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			char log[512];

			//Get the log and print it to console output.
			glGetShaderInfoLog(m_vertex_shader, 512, NULL, log);
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
	glDeleteShader(m_vertex_shader);
	glDeleteShader(m_fragment_shader);
}

ShaderProgram::~ShaderProgram()
{
	_clean_();
	glDeleteProgram(m_shader_program);
}
