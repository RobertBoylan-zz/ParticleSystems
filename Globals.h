#pragma once
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <cstdio>
#include <map>
#include <algorithm>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm/gtx/norm.hpp>

#define PI 3.14159265358979323846f
#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::perspective;
using namespace std;

enum ExitCodes
{
	EC_NO_ERROR = 0,
	EC_GLFW_INIT_FAIL = 1,
	EC_GLFW_FIRST_WINDOW_CREATION_FAIL = 2,
};