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
	/* Change the value of a parameter in the demo */
	void ChangeParameter();
	/* Pause the demo */
	void PauseDemo();
	/* Pick which demo to render */
	void PickDemo();

private:
	int mode;
	float gravity;
	bool pause;
	GLFWwindow* window;
	Camera camera;
	ParticleEmitter particle;
};
