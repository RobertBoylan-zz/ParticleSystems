#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#define PARTICLE_COUNT 10000

enum {
	SNOW_DEMO, RAIN_DEMO, STARS_DEMO, CLOUD_DEMO, FOUNTAIN_DEMO, SMOKE_DEMO, FIRE_DEMO
};

/* CPU representation of a particle */
struct Particle {
	
	float size, life, cameraDistance;
	vec3 position, speed;
	unsigned char red, green, blue, alpha;

	/* Sort in reverse order, far particles drawn first */
	bool operator<(const Particle& p) const {

		return this->cameraDistance > p.cameraDistance;
	}
};

class ParticleEmitter {

public:
	ParticleEmitter();

	/* Finds the next particle in the system that has not been used */
	int FindNextParticle(int mode);
	/* Load the particle's attributes */
	void LoadParticles(int mode);
	/* Draw each particle */
	void DrawParticles(GLFWwindow* window, int mode, float gravity, bool pause);
	/* Clear all of the data */
	void CleanUp(GLFWwindow* window);
	/* Gets the number of rows and columns in the texture sprite sheet */
	void TextureAtlas(int spriteIndex, const int numOfRows, const int numOfColumns);
	/* Changes the texture being binded to the particle, animating it */
	void AnimateTexture(float maxAge, double age, int numOfTextures);

private:
	int previousParticle, textureIndex;
	double lastTime;
	GLuint textureID, particleShaderProgramID, fireShaderProgramID, VAO, cameraRightID, cameraUpID, viewProjMatrixID, orientationID, vertexBuffer, positionBuffer, colourBuffer, numOfRowsID, offsetID;
	GLuint snowflakeTexture, raindropTexture, starTexture, cloudTexture, fireTexture;
	static GLfloat* particlePositionData;
	static GLubyte* particleColourData; 
	vec2 offset;
	vec3 windEffect;
	mat4 orientation;
	Particle particleList[PARTICLE_COUNT];
	Shader* shader;
	Camera camera;
	Texture texture;
};
