#version 330 core

in struct fragment_data
{
    vec3 position;
    vec3 normal;
	vec3 eye;
} fragment;

in float visibility;

layout(location=0) out vec4 FragColor;
uniform vec3 light = vec3(1.0, 1.0, 1.0);
uniform vec3 fogColor = {1.0, 1.0, 1.0};

uniform vec3 color = vec3(1.0, 1.0, 1.0); // Uniform color of the object
uniform float alpha = 1.0f; // alpha coefficient
uniform float Ka = 0.4; // Ambient coefficient
uniform float Kd = 0.8; // Diffuse coefficient
uniform float Ks = 0.4f;// Specular coefficient
uniform float specular_exp = 128.0; // Specular exponent

void main()
{
	vec3 N = normalize(fragment.normal);
	if (gl_FrontFacing == false) {
		N = -N;
	}
	vec3 L = normalize(light-fragment.position);

	float diffuse = max(dot(N,L),0.0);
	float specular = 0.0;
	if(diffuse>0.0){
		vec3 R = reflect(-L,N);
		vec3 V = normalize(fragment.eye-fragment.position);
		specular = pow( max(dot(R,V),0.0), specular_exp );
	}

	vec3 color_shading = (Ka + Kd * diffuse) * vec3(0.4f, 0.0f, 0.0f) + Ks * specular * vec3(1.0, 1.0, 1.0);
	vec3 color_with_fog = mix(fogColor, color_shading, visibility);

	FragColor = vec4(color_with_fog, alpha);
}