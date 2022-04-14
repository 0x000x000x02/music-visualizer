#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
		return m_total_vertices * sizeof(float);
	}
	int indices_size()
	{
		return m_total_indices * sizeof(unsigned);
	}

	int isize()
	{
		return m_total_indices;
	}
	int vsize()
	{
		return m_total_vertices;
	}

	virtual ~Object() = default;
protected:
	unsigned m_total_vertices = 0;
	unsigned m_total_indices = 0;

	float* m_vertices = nullptr;
	unsigned* m_indices = nullptr;
};

void Object::draw(GLFWwindow* window)
{
	glDrawElements(GL_TRIANGLES, m_total_indices, GL_UNSIGNED_INT, 0);
}

void Object::set_vertices(float* verts, int size)
{
	if (m_vertices)
		delete[] m_vertices;

	m_total_vertices = size;

	m_vertices = new float[size];
	for (size_t i = 0; i < size; ++i)
		m_vertices[i] = verts[i];
}

float* Object::get_vertices()
{
	return m_vertices;
}

void Object::set_indices(unsigned* indcs, int size)
{
	if (m_indices)
		delete[] m_indices;

	m_total_indices = size;

	m_indices = new unsigned[size];
	for (size_t i = 0; i < size; ++i)
		m_indices[i] = indcs[i];
}

unsigned* Object::get_indices()
{
	return m_indices;
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
	int m_width;
	int m_height;

	int m_pos_x;
	int m_pos_y;

	int m_x_roof;
	int m_y_roof;

	float m_r, m_g, m_b;
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
	return ((float)val)/(roof/static_cast<float>(2));
}

void Bar::set_roof(int x, int y)
{
	m_x_roof = x;
	m_y_roof = y;
}

void Bar::set_width(int val)
{
	m_width = val;
}

void Bar::set_height(int val)
{
	m_height = val;
}

void Bar::set_color(float red, float green, float blue)
{
	m_r = red;
	m_g = green;
	m_b = blue;
}

void Bar::set(int x, int y, int wdth, int hght)
{
	m_pos_x = x;
	m_pos_y = y;

	set_width(wdth);
	set_height(hght);

	float x_rm = remap(m_pos_x, m_x_roof) - 1.0f;
	float y_rm = remap(m_pos_y, m_y_roof) - 1.0f;
	float wdth_rm = remap(m_width, m_x_roof);
	float hght_rm = remap(m_height, m_y_roof);

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
	return m_r;
}

float Bar::get_color_g() const
{
	return m_g;
}

float Bar::get_color_b() const
{
	return m_b;
}