#include "scene.hpp"
#include "vector"
#include <WaveFunction.cpp>
#include <parameters.hpp>

using namespace cgp;


//std::vector<std::vector<cgp::mesh>> {

//}

// Camera controller
void scene_structure::update_camera(GLFWwindow* window, int width, int height)
{
	inputs_keyboard_parameters const& keyboard = inputs.keyboard;
	inputs_mouse_parameters const& mouse = inputs.mouse;
	camera_head& camera = environment.camera;

	// The camera moves forward all the time
	//   We consider in this example a constant velocity, so the displacement is: velocity * dt * front-camera-vector
	float const dt = timer.update();
	vec3 const forward_displacement = gui.speed * 0.1f * dt * camera.front() * (1 * keyboard.up - 1 * keyboard.down);
	vec3 const side_displacement = gui.speed * 0.1f * dt * camera.right() * (1 * keyboard.right - 1 * keyboard.left);
	vec3 const up_displacement = gui.speed * 0.1f * dt* vec3{ 0.0f, 0.0f, 1.0f} *(1 * keyboard.shift - 1 * keyboard.ctrl);

	camera.position_camera += forward_displacement + side_displacement + up_displacement;

	// The camera rotates following the mouse cursor
	

	float const pitch = 1.0f; // speed of the pitch
	float const yaw = 1.0f; // speed of the yaw

	float pitch_displacement_desired = mouse.position.current.y;
	float yaw_displacement_desired =  - (mouse.position.current.x);

	camera.manipulator_rotate_roll_pitch_yaw(0, pitch_displacement_desired * pitch * dt, yaw_displacement_desired * yaw * dt);
}

// *********************************************** //
// Generating structure with WaveFunctionCollapse
// *********************************************** //

std::set<std::string> tiles = { "  ", "-'", "|_", "|'", "-."," +", "--", "| "};
std::map<const std::string, int> weights;
std::set<std::string> compatibilities[parameters::nTiles][parameters::nTiles];
std::map<std::string, int> numbers;

// Block type to array with possible mesh paths
std::map<int, std::vector<mesh>> modelMap = 
{
	{ 2, {mesh_load_file_obj("assets/upLeft.obj"), mesh_load_file_obj("assets/upLeftStairsUp.obj"), mesh_load_file_obj("assets/upLeftStairsDown.obj")}},
	{ 3, {mesh_load_file_obj("assets/upRight.obj"), mesh_load_file_obj("assets/upRightStairsUp.obj"), mesh_load_file_obj("assets/upRightStairsDown.obj")}},
	{ 0, {mesh_load_file_obj("assets/downRight.obj"), mesh_load_file_obj("assets/downRightStairsUp.obj"), mesh_load_file_obj("assets/downRightStairsDown.obj")}},
	{ 1, {mesh_load_file_obj("assets/downLeft.obj"), mesh_load_file_obj("assets/downLeftStairsUp.obj"), mesh_load_file_obj("assets/downLeftStairsDown.obj")}},
	{ 4, {mesh_load_file_obj("assets/upDownRightLeft.obj"), mesh_load_file_obj("assets/upDownRightLeftStairsUp.obj"), mesh_load_file_obj("assets/upDownRightLeftStairsDown.obj")}},
	{ 5, {mesh_load_file_obj("assets/rightLeftStairs.obj"), mesh_load_file_obj("assets/rightLeft.obj")}},
	{ 6, {mesh_load_file_obj("assets/upDownStairs.obj"), mesh_load_file_obj("assets/upDown.obj") }}
};

void initWFC() {

	// Tile weights
	weights.emplace("  ", 1);
	weights.emplace("-'", 15);
	weights.emplace("|_", 15);
	weights.emplace("|'", 15);
	weights.emplace("-.", 15);
	weights.emplace(" +", 1);
	weights.emplace("--", 100);
	weights.emplace("| ", 100);


	// Tile indices
	numbers.emplace("  ", 0);
	numbers.emplace("-'", 1);
	numbers.emplace("|_", 2);
	numbers.emplace("|'", 3);
	numbers.emplace("-.", 4);
	numbers.emplace(" +", 5);
	numbers.emplace("--", 6);
	numbers.emplace("| ", 7);



	// Compatibility matrix

	compatibilities[0][1] = { "up", "left" };
	compatibilities[1][0] = { "down", "right" };

	compatibilities[2][0] = { "down", "left" };
	compatibilities[2][1] = { "right" };
	compatibilities[0][2] = { "up", "right" };
	compatibilities[1][2] = { "left" };

	compatibilities[3][0] = { "up", "left" };
	compatibilities[3][1] = { "down", "right" };
	compatibilities[3][2] = { "down" };
	compatibilities[0][3] = { "down", "right" };
	compatibilities[1][3] = { "up", "left" };
	compatibilities[2][3] = { "up" };

	compatibilities[4][0] = { "up", "right" };
	compatibilities[4][1] = { "down" };
	compatibilities[4][2] = { "down", "left" };
	compatibilities[4][3] = { "left" };
	compatibilities[0][4] = { "down", "left" };
	compatibilities[1][4] = { "up" };
	compatibilities[2][4] = { "up", "right" };
	compatibilities[3][4] = { "right" };
	compatibilities[5][0] = {};
	compatibilities[5][1] = { "down", "right" };
	compatibilities[5][2] = { "down", "left" };
	compatibilities[5][3] = { "up", "left" };
	compatibilities[5][4] = { "up", "right" };
	compatibilities[0][5] = {};
	compatibilities[1][5] = { "up", "left" };
	compatibilities[2][5] = { "up", "right" };
	compatibilities[3][5] = { "down", "right" };
	compatibilities[4][5] = { "down", "left" };

	compatibilities[6][0] = { "up", "down" };
	compatibilities[6][1] = {"right" };
	compatibilities[6][2] = {"left" };
	compatibilities[6][3] = {"left" };
	compatibilities[6][4] = {"right" };
	compatibilities[6][5] = {"left", "right" };
	compatibilities[0][6] = { "up", "down" };
	compatibilities[1][6] = { "left" };
	compatibilities[2][6] = { "right" };
	compatibilities[3][6] = { "right" };
	compatibilities[4][6] = { "left" };
	compatibilities[5][6] = {"left", "right" };

	compatibilities[7][0] = { "left", "right" };
	compatibilities[7][1] = { "down" };
	compatibilities[7][2] = { "down" };
	compatibilities[7][3] = { "up" };
	compatibilities[7][4] = { "up" };
	compatibilities[7][5] = { "up", "down" };
	compatibilities[7][6] = {};
	compatibilities[0][7] = { "left", "right" };
	compatibilities[1][7] = { "up" };
	compatibilities[2][7] = { "up" };
	compatibilities[3][7] = { "down" };
	compatibilities[4][7] = { "down" };
	compatibilities[5][7] = { "up", "down" };
	compatibilities[6][7] = {};


}

mesh generate_structure()
{
	// WaveFunctionCollapse algorithm

	int size = parameters::gridSize;
	std::cout << "Starting WaveFunction" << std::endl;
	std::cout << "Begin init" << std::endl;
	initWFC();
	std::cout << "End init" << std::endl;
	std::cout << "Make compatibilityTester" << std::endl;
	CompatibilityTester compatibilityTester = CompatibilityTester(compatibilities, numbers);
	std::cout << "CompatibilityTester done" << std::endl;

	std::cout << "Make Propagator" << std::endl;
	Propagator propagator = Propagator(parameters::gridSize, weights, tiles, compatibilityTester);
	std::cout << "Propagator done" << std::endl;


	std::cout << "Start run" << std::endl;
	std::map<const std::pair<int, int>, std::string> output = propagator.run();
	std::cout << "End run" << std::endl;


	// Creates the mesh
	mesh structure;
	for (int i = size - 1; i >= 0; i--)
	{

		for (int j = 0; j < size; j++)
		{
			std::pair<int, int> currentCoord = std::make_pair(i, j);
			if (numbers[output[currentCoord]] != 0) {
				mesh block;
				int blockType = numbers[output[currentCoord]] - 1;
				int random = rand() % modelMap[blockType].size();
				block = modelMap[numbers[output[currentCoord]] - 1][random];
				block.position += vec3(10 * (i - size / 2) + 5, 10 * (j - size / 2) + 5, 0);
				structure.push_back(block);
			}
		}
	}

	return structure;
}

void scene_structure::initialize()
{
	// Create the hierarchy
	// ***************************************** //

	mesh room_mesh = mesh_load_file_obj("assets/room1.obj");
	room_mesh.uv *= 10;

	mesh_drawable room0;

	room0.initialize(room_mesh, "room0");
	room0.transform.scaling *= 50.0f;
	room0.texture = opengl_load_texture_image("assets/PencilTextureTore.png", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
	room0.shading.phong = {0.1f, 0.3f, 0.0f };

	mesh_drawable room1;

	room1.initialize(room_mesh, "room1");
	room1.transform.scaling *= 100.0f;
	room1.texture = opengl_load_texture_image("assets/PencilTextureTore.png", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
	room1.shading.phong = {0.1f, 0.3f, 0.0f };

	mesh_drawable room2;
	
	room2.initialize(room_mesh, "room2");
	room2.transform.scaling *= 180.0f/2;
	room2.transform.rotation = room2.transform.rotation.from_axis_angle(vec3{ 1.0f, 0.0f, 0.0f }, Pi);
	room2.texture = opengl_load_texture_image("assets/PencilTextureTore.png", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
	room2.shading.phong = {0.1f, 0.3f, 0.0f };

	room_mesh.uv *= 0.1f;

	mesh_drawable room3;

	room3.initialize(room_mesh, "room3");
	room3.transform.scaling *=  1;
	room3.transform.rotation = room3.transform.rotation.from_axis_angle(vec3{ 0.0f, 1.0f, 0.0f }, Pi);
	room3.texture = opengl_load_texture_image("assets/PencilTextureTore.png", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
	room3.shading.phong = { 0.04f, 0.04f, 0.0f };

	mesh_drawable structure;
	structure.initialize(generate_structure(), "structure");
	structure.texture = opengl_load_texture_image("assets/PencilTextureTore.png", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
	structure.shading.phong = { 0.1f, 0.3f, 0.0f };

	mesh_drawable structure0;
	structure0.initialize(generate_structure(), "structure0");
	structure0.texture = opengl_load_texture_image("assets/PencilTextureTore.png", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
	structure0.shading.phong = { 0.1f, 0.3f, 0.0f };

	// Generate the hierarchy (Currently unused) 
	hierarchy0.add(room0);
	hierarchy0.add(room1, "room0", { 0.0f, 0.0f, 0.0f });
	hierarchy0.add(room2, "room0", { 0.0f, 0.0f, 0.0f });
	hierarchy0.add(room3, "room0", { 0.0f, 0.0f, 0.0f });
	hierarchy0.add(structure, "room0", { 0.0f, 0.0f, -9.0f });
	hierarchy0.add(structure0, "room0", { 0.0f, 0.0f, 9.0f });

	structure.transform.translation = { 0.0f, 0.0f, 0.0f };

	// Implicit function initialisation
	// 
	// Load the shader used to display the implicit surface (only a polygon soup)
	GLuint shader_triangle_soup = opengl_load_shader("shaders/mesh_geometry/vert.glsl", "shaders/mesh_geometry/frag.glsl");
	triangle_soup_drawable::default_shader = shader_triangle_soup;

	// Initialize the field and the implicit surface
	implicit_surface.set_domain(20, { 20, 20, 20});
	implicit_surface.update_field(field_function);


	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");

	// Initial placement of the camera
	environment.camera.position_camera = { -75.0f, 0.0f, 5.0f };
}



void scene_structure::display()
{
	// Place the light at the camera position
	environment.light = environment.camera.position();

	if (gui.display_frame)      // Display the basic frame
		draw(global_frame, environment);


	// Animations
	float dt = 0.013;
	timer.update();
	float const t = timer.t;


	hierarchy0["room3"].transform.rotation = hierarchy0["room3"].transform.rotation.from_axis_angle(vec3{ sin(t), cos(t), 1.0f }, timer.t);

	/*
	// Flying structure wobble


	hierarchy0["structure0"].transform.translation = vec3{ 0,0,10.0f + 2.0f * std::cos(0.5f * t) };
	hierarchy0["structure"].transform.translation = vec3{ 0,0,-10.0f + 2.0f * std::cos(0.5f * t + Pi/2) };
	*/

	// Spring animation on the implicit surfaces
	float l = 0.5f;
	float k = 3.0f;
	for (size_t i = 0; i < sizeof(field_function.points)/ sizeof(*field_function.points); i++)
	{
		for (size_t j = sizeof(field_function.points) / sizeof(*field_function.points) - 1; i < j; j--) {
			vec3 ji = field_function.points[i] - field_function.points[j];
			float nji = std::sqrt(ji.x * ji.x + ji.y * ji.y + ji.z * ji.z);
			field_function.velocities[i] += k * (l - nji) * dt * ji / nji;
			field_function.velocities[j] -= k * (l - nji) * dt * ji / nji;
		}

	}
	for (size_t i = 0; i < sizeof(field_function.points) / sizeof(*field_function.points); i++)
	{
		field_function.points[i] += field_function.velocities[i] * dt;
	}

	implicit_surface.update_field(field_function);
	implicit_surface.update_marching_cube();
	
	//Draw calls


	// Structure
	// Update the global coordinate transformation
	hierarchy0.update_local_to_global_coordinates();
	draw(hierarchy0, environment);

	// Implicit functions
	draw(implicit_surface.drawable_param.shape, environment);
	//draw(implicit_surface.drawable_param.domain_box, environment);

	if (gui.display_wireframe) {
		draw_wireframe(hierarchy0, environment);
		draw_wireframe(implicit_surface.drawable_param.shape, environment, { 0,0,0 });
	}

}


void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
	ImGui::SliderFloat("Speed", &gui.speed, 50.0f, 500.0f); // Set the camera velocity

}