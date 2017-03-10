#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#define PARTICLE_COUNT 10000
#define FIRE_PARTICLE_COUNT 1

enum {
	SNOW_DEMO, RAIN_DEMO, STARS_DEMO, CLOUD_DEMO, FOUNTAIN_DEMO, SMOKE_DEMO, FIRE_DEMO
};

/* CPU representation of a particle */
struct Particle {
	
	vec3 position, speed;
	unsigned char red, green, blue, alpha;
	float size, life, camDistance; 

	/* Sort in reverse order, far particles drawn first */
	bool operator<(const Particle& p) const {

		return this->camDistance > p.camDistance;
	}
};

class ParticleEmitter {

public:
	ParticleEmitter();

	/* Finds the next particle in the system that has not been used */
	int FindNextParticle();
	/* Load the particle's attributes */
	void LoadParticles(int mode);
	/* Draw each particle */
	void DrawParticles(GLFWwindow* window, int mode);
	/* Clear all of the data */
	void CleanUp(GLFWwindow* window);
	/* Get a random float between two values */
	float RandomFloat(float min, float max);
	/* Gets the number of rows and columns in the texture sprite sheet */
	void TextureAtlas(int spriteIndex, const int numOfRows, const int numOfColumns);
	/* Changes the texture being binded to the particle, animating it */
	void AnimateTexture(float maxAge, double age, int numOfTextures);
	/* Returns the projection matrix */
	mat4 getProjectionMatrix() { return projectionMatrix; }
	/* Returns the view matrix */
	mat4 getViewMatrix() { return viewMatrix; }

private:
	int previousParticle, textureIndex;
	GLuint textureID, texture, shaderProgramID, VAO, cameraRightID, cameraUpID, viewProjMatrixID, orientationID, vertexBuffer, positionBuffer, colourBuffer, numOfRowsID, offsetID;
	static GLfloat* particlePositionData;
	static GLubyte* particleColourData; 
	vec2 offset;
	vec3 windEffect;
	mat4 projectionMatrix, viewMatrix, orientation;
	Particle particleList[PARTICLE_COUNT];
	Shader* shader;
	Camera camera;
	Texture particleTexture;
};
