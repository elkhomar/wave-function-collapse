#pragma once

#include "../curve_drawable/curve_drawable.hpp"

namespace cgp
{
	/** Extended curve_drawable that can handle dynamic addition of points using .push_back() 
	* Reserve some extra space in the VBO, and resize it if needed when adding new points.*/
	struct curve_dynamic_drawable : curve_drawable
	{
		curve_dynamic_drawable();

		curve_dynamic_drawable& clear();

		// Initialize the VBOs to store initial_capacity points
		curve_dynamic_drawable& initialize(std::string const& name, GLuint shader = default_shader, int initial_capacity = 100);

		// Add a new point in the VBO. 
		curve_dynamic_drawable& push_back(cgp::vec3 const& p);

		// Store the current capacity of the VBO. When the actual number of points reach the capacity, the VBO is resized to double capacity.
		int capacity;
	};
}