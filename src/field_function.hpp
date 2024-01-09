#pragma once

#include "cgp/cgp.hpp"

// Parametric function defined as a sum of blobs-like primitives
//  f(p) = sa exp(-||p-pa||^2) + sb exp(-||p-pb||^2) + sc exp(-||p-pc||^2)
struct field_function_structure {

	// Query the value of the function at any point p
	float operator()(cgp::vec3 const& p) const;


	// ***************************//
	// Parameters
	// ***************************//

	// The 5 center of the 5 blobs

	cgp::vec3 points[5] = { {1.0f, 1.0f, 1.0f},
							{1.0f, 1.0f, -1.0f },
							{-1.0f, -1.0f, 1.0f },
							{-1.0f, -1.0f, -1.0f },
							{1.5f, 1.5f, 1.5f } };

	cgp::vec3 velocities[5] = { {0.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f } };

	// The magnitude associated to each primitive

	float weights[5] = { 1.0f, 0.2f, 1.0f, 1.0f, 1.0f };
	float sizes[5] = { 1.2f, 1.0f, 1.2f, 1.0f, 1.0f };

};

