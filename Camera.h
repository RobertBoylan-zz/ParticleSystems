#pragma once
#include "Globals.h"

class Camera {

public:
	Camera();

	/* Changes the camera's location and view, given user input */
	void CameraInputs(GLFWwindow* window);
	/* Returns the camera position */
	vec3 getPosition() const { return position; }
	/* Returns the camera view direction */
	vec3 getViewDirection() const { return viewDirection; }
	/* Returns the camera projection matrix */
	mat4 getProjectionMatrix() { return projectionMatrix; }
	/* Returns the camera view matrix */
	mat4 getViewMatrix() { return viewMatrix; }
	/* Returns the distance the camera has travelled from frame-to-frame */
	vec3 getDeltaPosition() const { return deltaPosition; }

private:
	mat4 projectionMatrix, viewMatrix;
	vec3 position, viewDirection, UP, strafeDirection, initialPosition, deltaPosition;
	static const float moveSpeed, rotateSpeed;
	float yaw, pitch;
};
