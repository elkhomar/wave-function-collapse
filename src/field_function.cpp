#include "field_function.hpp"

using namespace cgp;

// Parameterization Gaussian centered at point p0
static float gaussian(vec3 const& p, vec3 const& p0, float sigma)
{
	float const d = norm(p - p0);
	float const value = std::exp(-(d * d) / (sigma * sigma));
	return value;
}

float field_function_structure::operator()(cgp::vec3 const& p) const
{
	float value = 0.0f;
	for (int i = 0; i < sizeof(points)/ sizeof(*points); i++)
	{
		value += weights[i]*gaussian(p, points[i], sizes[i]);
	}

	return value;
}
