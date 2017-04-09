#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\norm.hpp>

#define PI 3.14159265358979323846f
#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

using glm::vec2;
using glm::vec3;
using glm::mat4;
using namespace std;

static float RandomFloat(float min, float max) {

	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;

	return min + r;
}
