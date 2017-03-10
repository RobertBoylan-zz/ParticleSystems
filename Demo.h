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

private:
	int mode;
	GLFWwindow* window;
	Camera camera;
	ParticleEmitter particle;
};