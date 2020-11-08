#pragma once

#include <iostream>

#include <GLAD/glad.h>
#include <glfw/glfw3.h>

class Object 
{
public:
	Object() = default;

	virtual void draw(GLFWwindow* window);

	void set_vertices(float *, int size);
	void set_indices(unsigned *, int size);
	float* get_vertices();
	unsigned* get_indices();

	int vertices_size()
	{
		return total_vertices * sizeof(float);
	}
	int indices_size()
	{
		return total_indices * sizeof(unsigned);
	}

	int isize()
	{
		return total_indices;
	}
	int vsize()
	{
		return total_vertices;
	}

	virtual ~Object() = default;
protected:
	unsigned total_vertices = 0;
	unsigned total_indices = 0;

	float* _vertices = nullptr;
	unsigned* _indices = nullptr;
};

void Object::draw(GLFWwindow* window)
{
	glDrawElements(GL_TRIANGLES, total_indices, GL_UNSIGNED_INT, 0);
}

void Object::set_vertices(float* verts, int size)
{
	if (_vertices)
		delete[] _vertices;

	total_vertices = size;

	_vertices = new float[size];
	for (size_t i = 0; i < size; ++i)
		_vertices[i] = verts[i];
}

float* Object::get_vertices()
{
	return _vertices;
}

void Object::set_indices(unsigned* indcs, int size)
{
	if (_indices)
		delete[] _indices;

	total_indices = size;

	_indices = new unsigned[size];
	for (size_t i = 0; i < size; ++i)
		_indices[i] = indcs[i];
}

unsigned* Object::get_indices()
{
	return _indices;
}

/*BAR object class*/
class Bar : public Object
{
public:
	Bar();
	Bar(float wdth, float hght);

	void set_roof(int x, int y);
	void set_width(int wdth);
	void set_height(int hght);
	void set_color(float red, float green, float blue);
	void set(int x, int y, int wdth, int hght);

	float get_color_r() const;
	float get_color_g() const;
	float get_color_b() const;

protected:
	void init();
	float remap(int, int);

private:
	int width;
	int height;

	int pos_x;
	int pos_y;

	int x_roof;
	int y_roof;

	float r, g, b;
};

Bar::Bar()
{
	init();
}

void Bar::init()
{
	unsigned indcs[] =
	{
		0, 1, 2,
		1, 2, 3
	};

	this->set_indices(indcs, 6);
}

float Bar::remap(int val, int roof)
{
	//std::cout << ((float)val / roof) << std::endl;
	return ((float)val / (roof/2));
}

void Bar::set_roof(int x, int y)
{
	x_roof = x;
	y_roof = y;
}

void Bar::set_width(int val)
{
	width = val;
}

void Bar::set_height(int val)
{
	height = val;
}

void Bar::set_color(float red, float green, float blue)
{
	r = red;
	g = green;
	b = blue;
}

void Bar::set(int x, int y, int wdth, int hght)
{
	pos_x = x;
	pos_y = y;

	set_width(wdth);
	set_height(hght);

	float x_rm = remap(pos_x, x_roof) - 1.0f;
	float y_rm = remap(pos_y, y_roof) - 1.0f;
	float wdth_rm = remap(width, x_roof);
	float hght_rm = remap(height, y_roof);

	float verts[] =
	{
		x_rm, y_rm, 0.0f,
		(x_rm + wdth_rm), y_rm, 0.0f,
		x_rm, (y_rm + hght_rm), 0.0f,
		(x_rm + wdth_rm), (y_rm + hght_rm), 0.0f
	};

#ifdef _DEBUG
	//std::cout << "Vertices size: " << sizeof(verts) / sizeof(float);
#endif

	this->set_vertices(verts, sizeof(verts)/sizeof(float));
}

float Bar::get_color_r() const
{
	return r;
}

float Bar::get_color_g() const
{
	return g;
}

float Bar::get_color_b() const
{
	return b;
}