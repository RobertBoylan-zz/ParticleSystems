#include "ParticleEmitter.h"

GLfloat* ParticleEmitter::particlePositionData = new GLfloat[4 * PARTICLE_COUNT];
GLubyte* ParticleEmitter::particleColourData = new GLubyte[4 * PARTICLE_COUNT];

ParticleEmitter::ParticleEmitter(){

	previousParticle = textureIndex = 0;
	offset.x = offset.y = RandomFloat(-0.5f, 0.5f);
}

int ParticleEmitter::FindNextParticle(int mode) {

	if (mode == FIRE_DEMO) {
		for (int i = previousParticle; i < (PARTICLE_COUNT / PARTICLE_COUNT); i++) {
			if (particleList[i].life < 0) {
				previousParticle = i;
				return i;
			}
		}
	}
	else {
		for (int i = previousParticle; i < PARTICLE_COUNT; i++) {
			if (particleList[i].life < 0) {
				previousParticle = i;
				return i;
			}
		}
	}

	for (int j = 0; j < previousParticle; j++) {
		if (particleList[j].life < 0) {
			previousParticle = j;
			return j;
		}
	}

	/* All particles are taken, override the first one */
	return 0;
}

void ParticleEmitter::LoadParticles(int mode) {
	
	/* Background colour */
	if (mode == SMOKE_DEMO) {
		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else {

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	/* Enable depth test */
	glEnable(GL_DEPTH_TEST);
	/* Accept fragment if it closer to the camera than the previous one */
	glDepthFunc(GL_LESS);

	if (mode == FIRE_DEMO) {

		/* Create and compile the GLSL program from the shaders */
		shader = new Shader(TEXTURE_ATLAS_VERTEX_SHADER, TEXTURE_ATLAS_FRAGMENT_SHADER);
		shaderProgramID = shader->CompileShaders();

		numOfRowsID = glGetUniformLocation(shaderProgramID, "numOfRows");
		offsetID = glGetUniformLocation(shaderProgramID, "offset");
	}

	else {

		/* Create and compile the GLSL program from the shaders */
		shader = new Shader(VERTEX_SHADER, FRAGMENT_SHADER);
		shaderProgramID = shader->CompileShaders();
	}

	cameraRightID = glGetUniformLocation(shaderProgramID, "cameraRight");
	cameraUpID = glGetUniformLocation(shaderProgramID, "cameraUp");
	viewProjMatrixID = glGetUniformLocation(shaderProgramID, "viewProjectionMatrix");
	orientationID = glGetUniformLocation(shaderProgramID, "orientation");
	textureID = glGetUniformLocation(shaderProgramID, "myTexture");

	/* Load textures */
	if (mode == SNOW_DEMO) {
		texture = particleTexture.LoadDDSTexture(SNOWFLAKE_TEXTURE);

		for (int i = 0; i < PARTICLE_COUNT; i++) {
			particleList[i].life = -1.0f;
			particleList[i].camDistance = -1.0f;
		}
	}
	else if (mode == STARS_DEMO) {
		texture = particleTexture.LoadDDSTexture(STAR_TEXTURE);

		for (int i = 0; i < PARTICLE_COUNT; i++) {
			particleList[i].life = -1.0f;
			particleList[i].camDistance = -1.0f;
		}
	}
	else if (mode == RAIN_DEMO || mode == FOUNTAIN_DEMO) {
		texture = particleTexture.LoadDDSTexture(RAINDROP_TEXTURE);

		for (int i = 0; i < PARTICLE_COUNT; i++) {
			particleList[i].life = -1.0f;
			particleList[i].camDistance = -1.0f;
		}
	}
	else if (mode == CLOUD_DEMO) {
		texture = particleTexture.LoadDDSTexture(CLOUD_TEXTURE);

		for (int i = 0; i < PARTICLE_COUNT; i++) {
			particleList[i].life = -1.0f;
			particleList[i].camDistance = -1.0f;
		}
	}
	else if (mode == SMOKE_DEMO) {
		texture = particleTexture.LoadDDSTexture(CLOUD_TEXTURE);

		for (int i = 0; i < PARTICLE_COUNT; i++) {
			particleList[i].life = -1.0f;
			particleList[i].camDistance = -1.0f;
		}
	}
	else if (mode == FIRE_DEMO) {
		texture = particleTexture.LoadDDSTexture(FIRE_TEXTURE);

		for (int i = 0; i < (PARTICLE_COUNT / PARTICLE_COUNT); i++) {
			particleList[i].life = -1.0f;
			particleList[i].camDistance = -1.0f;
		}
	}

	/* The VBO containing the 4 vertices of the particles that will be shared by all particles */
	static const GLfloat vertexBufferData[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	if (mode == FIRE_DEMO) {
		vertexBuffer = VAO;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

		/* The VBO containing the positions and sizes of the particles */
		positionBuffer = VAO;
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		/* Initialize with empty buffer - it will be updated each frame */
		glBufferData(GL_ARRAY_BUFFER, 4 * (PARTICLE_COUNT / PARTICLE_COUNT) * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		/* The VBO containing the colours of the particles */
		colourBuffer = VAO;
		glGenBuffers(1, &colourBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
		/* Initialize with empty buffer - it will be updated each frame */
		glBufferData(GL_ARRAY_BUFFER, 4 * (PARTICLE_COUNT / PARTICLE_COUNT) * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	}

	else {
		vertexBuffer = VAO;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

		/* The VBO containing the positions and sizes of the particles */
		positionBuffer = VAO;
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		/* Initialize with empty buffer - it will be updated each frame */
		glBufferData(GL_ARRAY_BUFFER, 4 * PARTICLE_COUNT * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		/* The VBO containing the colours of the particles */
		colourBuffer = VAO;
		glGenBuffers(1, &colourBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
		/* Initialize with empty buffer - it will be updated each frame */
		glBufferData(GL_ARRAY_BUFFER, 4 * PARTICLE_COUNT * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void ParticleEmitter::DrawParticles(GLFWwindow* window, int mode) {

	double lastTime = glfwGetTime();
	
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_1) != GLFW_PRESS && !glfwWindowShouldClose(window)) {
		
		/* Clear the screen */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		double age = currentTime - lastTime;
		lastTime = currentTime;

		camera.CameraInputs(window);
		projectionMatrix = camera.getProjectionMatrix();
		viewMatrix = camera.getViewMatrix();
		
		mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		vec3 CameraPosition(inverse(viewMatrix)[3]);

		/* Generate a new particle each millisecond but limit to 16 ms (60 fps) */
		int newParticle = (int)(age*1000.0f);

		if (newParticle > (int)(0.016f*1000.0f)) {
			
			newParticle = (int)(0.016f*1000.0f);
		}

		for (int i = 0; i < newParticle; i++) {
			
			int index = FindNextParticle(mode);
			float spread;
			vec3 mainDirection, randomDirection;
			vec3 deltaPosition = camera.getDeltaPosition();

			if (mode == SNOW_DEMO) {
				
				spread = 1.5f;
				mainDirection = vec3(0.0f, -10.0f, 0.0f);
				randomDirection = vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f)); 
				particleList[index].position = vec3(RandomFloat(-20.0f + deltaPosition.x, 20.0f + deltaPosition.x), RandomFloat(18.0f, 23.0f), RandomFloat(-20.0f + deltaPosition.z, 20.0f + deltaPosition.z));
				particleList[index].red = 255.0f;
				particleList[index].green = 255.0f;
				particleList[index].blue = 255.0f;
				particleList[index].size = RandomFloat(0.1f, 0.7f);
				particleList[index].alpha = (RandomFloat(0.0f, 255.0f)) / 3;
				particleList[index].life = 5.0f;
			}

			else if (mode == RAIN_DEMO) {

				spread = 1.5f;
				mainDirection = vec3(0.0f, -15.0f, 0.0f);
				randomDirection = vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f));
				particleList[index].position = vec3(RandomFloat(-20.0f + deltaPosition.x, 20.0f + deltaPosition.x), RandomFloat(18.0f, 23.0f), RandomFloat(-20.0f + deltaPosition.z, 20.0f + deltaPosition.z));
				particleList[index].red = 50.0f;
				particleList[index].green = 200.0f;
				particleList[index].blue = 255.0f;
				particleList[index].size = RandomFloat(0.01f, 0.2f);
				particleList[index].alpha = (RandomFloat(0.0f, 255.0f)) / 3;
				particleList[index].life = 5.0f;
			}

			else if (mode == STARS_DEMO) {
				
				spread = 0.0f;
				mainDirection = vec3(0.0f, 0.0f, 15.0f);
				randomDirection = vec3(0.0f, 0.0f, 0.0f); 
				particleList[index].position = vec3(RandomFloat(-15.0f + deltaPosition.x, 15.0f + deltaPosition.x), RandomFloat(-15.0f + deltaPosition.y, 15.0f + deltaPosition.y), RandomFloat(-25.0f + deltaPosition.z, -20.0f + deltaPosition.z));
				particleList[index].red = 255.0f;
				particleList[index].green = 255.0f;
				particleList[index].blue = RandomFloat(0.0f, 255.0f);
				particleList[index].size = RandomFloat(0.08f, 0.5f);
				particleList[index].alpha = (RandomFloat(0.0f, 255.0f)) / 3;
				particleList[index].life = 5.0f;
			}
			
			else if (mode == CLOUD_DEMO) {

				spread = 0.0f;
				mainDirection = vec3(1.0f, 0.0f, 0.0f);
				randomDirection = vec3(0.0f, 0.0f, 0.0f);
				particleList[index].position = vec3(-5.0f, RandomFloat(-0.5f + offset.y, 0.5f + offset.y), RandomFloat(-4.5f + offset.x, -3.5f + offset.x));
				particleList[index].red = 255;
				particleList[index].green = 255;
				particleList[index].blue = 255;
				particleList[index].size = RandomFloat(0.1f, 0.7f);
				particleList[index].alpha = (RandomFloat(215.0f, 255.0f)) / 3;
				particleList[index].life = RandomFloat(6.0f, 7.0f);
			}

			else if (mode == FOUNTAIN_DEMO) {
				
				spread = 1.5f;
				mainDirection = vec3(0.0f, 4.0f, 0.0f);
				randomDirection = vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f)); 
				particleList[index].position = vec3(0.0f, 0.0f, -2.0f);
				particleList[index].red = 50.0f;
				particleList[index].green = 200.0f;
				particleList[index].blue = 255.0f;
				particleList[index].size = RandomFloat(0.003f, 0.03f);
				particleList[index].alpha = (RandomFloat(0.0f, 255.0f)) / 3;
				particleList[index].life = 5.0f;
			}

			else if (mode == SMOKE_DEMO) {
				
				spread = RandomFloat(0.01f, 0.07f);
				mainDirection = vec3(RandomFloat(-0.4f, 0.4f), 2.0f, RandomFloat(-0.4f, 0.4f));
				randomDirection = vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f));
				particleList[index].position = vec3(RandomFloat(-0.2f + offset.x, 0.2f + offset.x), 0.0f, RandomFloat(-10.2f + offset.y, -9.8f + offset.y));
				particleList[index].red = 70.0f;
				particleList[index].green = 70.0f;
				particleList[index].blue = 70.0f;
				particleList[index].size = RandomFloat(0.3f, 0.6f);
				particleList[index].alpha = RandomFloat(215.0f, 255.0f) / 3;
				particleList[index].life = RandomFloat(7.0f, 8.0f);
			}

			else if (mode == FIRE_DEMO) {
				
				spread = 0.0f;
				mainDirection = vec3(0.0f, 0.0f, 0.0f);
				randomDirection = vec3(0.0f, 0.0f, 0.0f);
				particleList[index].position = vec3(0.0f, 0.0f, -5.0f);
				particleList[index].red = 0.0f;
				particleList[index].green = 0.0f;
				particleList[index].blue = 0.0f;
				particleList[index].size = 0.8f;
				particleList[index].alpha = 255.0f;
				particleList[index].life = 2.0f;
			}

			particleList[index].speed = mainDirection + randomDirection*spread;
		}

		/* Simulate all particles */
		int particleCount = 0;

		if (mode == FIRE_DEMO) {

			for (int i = 0; i < (PARTICLE_COUNT / PARTICLE_COUNT); i++) {

				Particle& particle = particleList[i];

				if (particle.life > 0.0f) {

					/* Decrease life */
					particle.life -= age;

					if (particle.life > 0.0f) {

						/* Simulate physics */
						particle.speed += vec3(0.0f, 0.0f, 0.0f) * (float)age * 0.5f;
						particle.position += particle.speed * (float)age;
						particle.camDistance = length2(particle.position - camera.getPosition());

						/* Fill the GPU buffer */
						particlePositionData[4 * particleCount + 0] = particle.position.x;
						particlePositionData[4 * particleCount + 1] = particle.position.y;
						particlePositionData[4 * particleCount + 2] = particle.position.z;
						particlePositionData[4 * particleCount + 3] = particle.size;

						particleColourData[4 * particleCount + 0] = particle.red;
						particleColourData[4 * particleCount + 1] = particle.green;
						particleColourData[4 * particleCount + 2] = particle.blue;
						particleColourData[4 * particleCount + 3] = particle.alpha;
					}

					else {
						/* Particles that just died will be put at the end of the buffer */
						particle.camDistance = -1.0f;
					}
					particleCount++;
				}
			}
			AnimateTexture(0.7f, age, 16);

			sort(&particleList[0], &particleList[(PARTICLE_COUNT / PARTICLE_COUNT)]);

			glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
			/* Buffer orphaning */
			glBufferData(GL_ARRAY_BUFFER, 4 * (PARTICLE_COUNT / PARTICLE_COUNT) * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * particleCount * sizeof(GLfloat), particlePositionData);

			glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
			glBufferData(GL_ARRAY_BUFFER, 4 * (PARTICLE_COUNT / PARTICLE_COUNT) * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * particleCount * sizeof(GLubyte), particleColourData);
		}

		else {
			for (int i = 0; i < PARTICLE_COUNT; i++) {

				Particle& particle = particleList[i];

				if (particle.life > 0.0f) {

					/* Decrease life */
					particle.life -= age;

					if (particle.life > 0.0f) {

						/* Simulate physics */
						if (mode == SNOW_DEMO) {
							particle.speed += vec3(0.0f, -9.81f, 0.0f) * (float)age * 0.5f;
						}
						else if (mode == RAIN_DEMO) {
							particle.speed += vec3(0.0f, -9.81f, 0.0f) * (float)age * 0.5f;
						}
						else if (mode == STARS_DEMO) {
							particle.speed += vec3(0.0f, 0.0f, 0.0f) * (float)age * 0.5f;
						}
						else if (mode == CLOUD_DEMO) {
							particle.speed += vec3(0.0f, 0.0f, 0.0f) * (float)age * 0.5f;

							if (particle.position.z <= -2.5f && particle.position.z >= -5.5f) {
								offset.x += RandomFloat(-0.0001f, 0.0001f);
							}

							if (particle.position.y <= 1.5f && particle.position.y >= -1.5f) {
								offset.y += RandomFloat(-0.0001f, 0.0001f);
							}
						}

						else if (mode == FOUNTAIN_DEMO) {
							particle.speed += vec3(0.0f, -9.81f, 0.0f) * (float)age * 0.5f;
						}
						else if (mode == SMOKE_DEMO) {

							if ((int)currentTime % 5 == 0) {

								windEffect.x = (RandomFloat(-0.6f, 0.6f));
								windEffect.z = (RandomFloat(-0.6f, 0.6f));
							}

							if (particle.position.x >= -0.2f && particle.position.x <= 0.2f) {
								offset.x += RandomFloat(-0.001f, 0.001f);
							}

							if (particle.position.z >= -10.2f && particle.position.z <= 9.8f) {
								offset.y += RandomFloat(-0.001f, 0.001f);
							}

							particle.speed += vec3(windEffect.x, 0.0f, windEffect.z) * (float)age * 0.5f;
						}

						particle.position += particle.speed * (float)age;
						particle.camDistance = length2(particle.position - camera.getPosition());

						/* Fill the GPU buffer */
						particlePositionData[4 * particleCount + 0] = particle.position.x;
						particlePositionData[4 * particleCount + 1] = particle.position.y;
						particlePositionData[4 * particleCount + 2] = particle.position.z;
						particlePositionData[4 * particleCount + 3] = particle.size;

						particleColourData[4 * particleCount + 0] = particle.red;
						particleColourData[4 * particleCount + 1] = particle.green;
						particleColourData[4 * particleCount + 2] = particle.blue;
						particleColourData[4 * particleCount + 3] = particle.alpha;
					}

					else {
						/* Particles that just died will be put at the end of the buffer */
						particle.camDistance = -1.0f;
					}
					particleCount++;
				}
			}
			sort(&particleList[0], &particleList[PARTICLE_COUNT]);

			glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
			glBufferData(GL_ARRAY_BUFFER, 4 * PARTICLE_COUNT * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
			glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * particleCount * sizeof(GLfloat), particlePositionData);

			glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
			glBufferData(GL_ARRAY_BUFFER, 4 * PARTICLE_COUNT * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
			glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * particleCount * sizeof(GLubyte), particleColourData);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/* Use the shader */
		glUseProgram(shaderProgramID);

		/* Bind the texture in Texture Unit 0 */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUniform1i(textureID, 0);
		glUniform3f(cameraRightID, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
		glUniform3f(cameraUpID, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
		glUniformMatrix4fv(viewProjMatrixID, 1, GL_FALSE, &viewProjectionMatrix[0][0]);
		glUniformMatrix4fv(orientationID, 1, GL_FALSE, &orientation[0][0]);

		/* First attribute buffer - vertices */
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Second attribute buffer - positions of particle centres */
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Third attribute buffer - particle colours */
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

		/* Particle vertices - always reuse the same 4 vertices -> 0 */
		glVertexAttribDivisor(0, 0);
		/* Positions - one per quad (its center) -> 1 */
		glVertexAttribDivisor(1, 1);
		/* Colour - one per quad -> 1 */
		glVertexAttribDivisor(2, 1);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		/* Swap buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void ParticleEmitter::CleanUp(GLFWwindow* window) {

	/* Cleanup VBO and shader, cleanup steps are generally performed in reverse order to setup steps */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &colourBuffer);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	delete[] particlePositionData;
	glDeleteTextures(1, &textureID);
	glDeleteProgram(shaderProgramID);
}

void ParticleEmitter::TextureAtlas(int spriteIndex, const int numOfRows, const int numOfColumns) {
	
	int col = spriteIndex % numOfColumns;
	int row = numOfRows - (spriteIndex / numOfColumns) - 1;
	
	float x = (float)col / (float)numOfColumns;
	float y = (float)row / (float)numOfRows;
	
	/* Use the shader */
	glUseProgram(shaderProgramID);
	glUniform2f(offsetID, x, y);
	glUniform1f(numOfRowsID, numOfRows);
}

void ParticleEmitter::AnimateTexture(float maxAge, double age, int numOfTextures) {

	static double timer = 0.0;
	timer += age;

	if (timer > maxAge) {
		timer -= maxAge;
		textureIndex = 0;
	}
	if (timer > (((textureIndex + 1) * maxAge) / numOfTextures) && textureIndex < numOfTextures) {
		TextureAtlas(textureIndex, 4, 4);
		textureIndex++;
	}
}

float ParticleEmitter::RandomFloat(float min, float max) {

	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;

	return min + r;
}