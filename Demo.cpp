#include "Demo.h"

Demo::Demo() {}

int Demo::CreateDemoWindow(int _mode) {

	mode = _mode;

	/* Open a window and create its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ParticleSystem", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/* Initialize GLEW */
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	/* Ensure we can capture the escape key being pressed below */
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	/* Hide the mouse and enable unlimited movement */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Set the mouse at the center of the screen */
	glfwPollEvents();
	glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

void Demo::DemoInitialize() {

	if (mode == SNOW_DEMO) {
		particle.LoadParticles(mode);
	}
	else if (mode == RAIN_DEMO) {
		particle.LoadParticles(mode);
	}
	else if (mode == STARS_DEMO) {
		particle.LoadParticles(mode);
	}
	else if (mode == CLOUD_DEMO) {
		particle.LoadParticles(mode);
	}
	else if (mode == FOUNTAIN_DEMO) {
		particle.LoadParticles(mode);
	}
	else if (mode == SMOKE_DEMO) {
		particle.LoadParticles(mode);
	}
	else if (mode == FIRE_DEMO) {
		particle.LoadParticles(mode);
	}
}

void Demo::DemoDisplay() {

	if (mode == SNOW_DEMO) {
		particle.DrawParticles(window, mode);
	}
	else if (mode == RAIN_DEMO) {
		particle.DrawParticles(window, mode);
	}
	else if (mode == STARS_DEMO) {
		particle.DrawParticles(window, mode);
	}
	else if (mode == CLOUD_DEMO) {
		particle.DrawParticles(window, mode);
	}
	else if (mode == FOUNTAIN_DEMO) {
		particle.DrawParticles(window, mode);
	}
	else if (mode == SMOKE_DEMO) {
		particle.DrawParticles(window, mode);
	}
	else if (mode == FIRE_DEMO) {
		particle.DrawParticles(window, mode);
	}
}

void Demo::CleanUp() {

	particle.CleanUp(window);
}