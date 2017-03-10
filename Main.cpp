/* Please note that to change to the different demos,
   simply copy the desired enum from ParticleEmitter.h
   and paste it into the CreateDemoWindow() function
   below. */

#include "Demo.h"

Demo demo;

/* Main Function */
int main(int argc, char** argv) {

	/* Initialise GLFW */
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	/* Creates the initial window */
	demo.CreateDemoWindow(SNOW_DEMO);

	/* Loads the particles and texture etc. */
	demo.DemoInitialize();

	/* Displays the particle system */
	demo.DemoDisplay();

	/* Clear all of the data */
	demo.CleanUp();

	/* Close OpenGL window and terminate GLFW */
	glfwTerminate();
	
	return 0;
}