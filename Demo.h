#pragma once
#include "ParticleEmitter.h"

class Demo {

public:
	Demo();
	
	/* Creates the initial window */
	int CreateDemoWindow(int _mode);
	/* Load the particles of the corresponding demo */
	void DemoInitialize(); 
	/* Display the corresponding demo */
	void DemoDisplay();
	/* Clear all of the data */
	void CleanUp();
	/* Performs and action given a keyboard input */
	void KeyboardOperations();

private:
	int mode;
	float gravity, particlesPerSecond;
	bool pause, whiteBackground;
	vec3 cameraPosition, fountainDirection;
	GLFWwindow* window;
	Camera camera;
	ParticleEmitter particle;
};
