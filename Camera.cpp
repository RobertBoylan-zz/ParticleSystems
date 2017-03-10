#include "Camera.h"

const float Camera::moveSpeed = 3.0f;
const float Camera::rotateSpeed = 0.005f;

Camera::Camera() :
	position(0.0f, 0.0f, 0.0f),
	viewDirection(0.0f, 0.0f, -1.0f),
	UP(0.0f, 1.0f, 0.0f)
{
	yaw = PI;
	pitch = 0.0f;
	initialPosition = position;
}

void Camera::CameraInputs(GLFWwindow* window) {

	/* glfwGetTime is called only once, the first time this function is called */
	static double lastTime = glfwGetTime();

	/* Compute time difference between current and last frame */
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	/* Get mouse position */
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	/* Reset mouse position for next frame */
	glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	/* Compute new orientation */
	yaw += rotateSpeed * float(WINDOW_WIDTH / 2 - xPos);
	pitch += rotateSpeed * float(WINDOW_HEIGHT / 2 - yPos);

	/* Set limit for the pitch */
	const float limit = 89.0 * PI / 180.0;

	if (pitch < -limit) {
		pitch = -limit;
	}

	if (pitch > limit) {
		pitch = limit;
	}

	/* Direction - Spherical coordinates to Cartesian coordinates conversion */
    viewDirection = vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));

	/* Right vector */
	vec3 right = vec3(sin(yaw - PI / 2.0f), 0, cos(yaw - PI / 2.0f));

	/* Up vector */
	UP = cross(right, viewDirection);

	/* Move forward */
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += viewDirection * deltaTime * moveSpeed;
		deltaPosition = position - initialPosition;
	}
	/* Move backward */
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= viewDirection * deltaTime * moveSpeed;
		deltaPosition = position - initialPosition;
	}
	/* Strafe right */
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * moveSpeed;
		deltaPosition = position - initialPosition;
	}
	/* Strafe left */
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * moveSpeed;
		deltaPosition = position - initialPosition;
	}

	projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	/* Camera matrix */
	viewMatrix = glm::lookAt(position, position + viewDirection, UP);

	/* For the next frame, the "last time" will be "now" */
	lastTime = currentTime;
}