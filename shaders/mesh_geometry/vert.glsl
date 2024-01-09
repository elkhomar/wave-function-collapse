#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out struct fragment_data
{
    vec3 position;
    vec3 normal;
	vec3 eye;
} fragment;

out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float density = 0.02f;
const float gradient = 1.5f;

void main()
{

	// The position of the vertex in the world space
	vec4 p = model * vec4(position, 1.0);
	vec4 positionRelativeToCam = view * p;
	// The normal of the vertex in the world space
	vec4 n = model * vec4(normal, 0.0);
	// The projected position of the vertex in the normalized device coordinates:
	vec4 p_proj = projection * positionRelativeToCam;

	fragment.position = vec3(model * vec4(position,1.0));
	fragment.normal   = vec3(model * vec4(normal  ,0.0));
	fragment.eye = vec3(inverse(view)*vec4(0,0,0,1.0));

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow(distance*density, gradient));
	visibility = clamp(visibility, 0.0f, 1.0f);

	gl_Position = projection * view * model * vec4(position, 0.7);
}