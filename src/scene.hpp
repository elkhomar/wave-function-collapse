#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera_head/environment_camera_head.hpp"
#include <iostream> 
#include <list>
#include "implicit_surface.hpp"



// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame     = false;
	bool display_wireframe = false;
	float speed = 50.0f;
};



// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	scene_environment_camera_head environment;

	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	gui_parameters gui;                       // Standard GUI element storage

	cgp::timer_basic timer;
	cgp::hierarchy_mesh_drawable hierarchy0;
	cgp::hierarchy_mesh_drawable hierarchy1;
	cgp::mesh_drawable structure;
	cgp::mesh_drawable structure0;

	implicit_surface_structure implicit_surface; // Structures used for the implicit surface (*)
	field_function_structure field_function;     // A Parametric function used to generate the discrete field (*)

	// ****************************** //
	// Functions
	// ****************************** //

	void update_camera(GLFWwindow* window,int width, int height);

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
};





