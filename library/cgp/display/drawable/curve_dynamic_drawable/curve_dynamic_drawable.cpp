#include "cgp/base/base.hpp"
#include "curve_dynamic_drawable.hpp"

namespace cgp
{
	curve_dynamic_drawable::curve_dynamic_drawable()
		:curve_drawable(), capacity(0)
	{}

	curve_dynamic_drawable& curve_dynamic_drawable::clear()
	{
		curve_drawable::clear();
		capacity = 0;

		return *this;
	}
	curve_dynamic_drawable& curve_dynamic_drawable::initialize(std::string const& name, GLuint shader, int initial_capacity)
	{
		buffer<vec3> temp(initial_capacity); // dummy vector of size initial_capacity (values are not used)
		curve_drawable::initialize(temp, name, shader);
		capacity = initial_capacity;
		number_position = 0;

		return *this;
	}

	curve_dynamic_drawable& curve_dynamic_drawable::push_back(cgp::vec3 const& p)
	{
		if (capacity <= int(number_position)) // Need resizing
		{
			// Future capacity
			int new_capacity = 2 * capacity;

			// Create the new VBO
			GLuint new_vbo = 0;
			glGenBuffers(1, &new_vbo);                                                                           opengl_check
				glBindBuffer(GL_ARRAY_BUFFER, new_vbo);                                                              opengl_check
				glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(new_capacity * 3 * sizeof(float)), nullptr, GL_DYNAMIC_DRAW);   opengl_check
				glBindBuffer(GL_ARRAY_BUFFER, 0);                                                                    opengl_check

				// Copy old VBO into new one
				GLint size_to_copy = capacity * 3 * sizeof(float);
			glBindBuffer(GL_COPY_READ_BUFFER, vbo_position); opengl_check;
			glBindBuffer(GL_COPY_WRITE_BUFFER, new_vbo); opengl_check;
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size_to_copy); opengl_check;
			glBindBuffer(GL_COPY_READ_BUFFER, 0);
			glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

			// Delete old VBO
			glDeleteBuffers(1, &vbo_position); opengl_check;

			// Switch to new VBO
			vbo_position = new_vbo;
			capacity = new_capacity;

			// Update the VAO with the new VBO
			glBindVertexArray(vao);     opengl_check
				opengl_set_vertex_attribute(vbo_position, 0, 3, GL_FLOAT);
			glBindVertexArray(0);       opengl_check

		}


		// Add a new position in the VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position);                                                   opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER, number_position * 3 * sizeof(float), 3 * sizeof(float), ptr(p));  opengl_check;
		number_position++;


		return *this;
	}


}