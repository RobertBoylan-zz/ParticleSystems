#include "ParticleEmitter.h"

GLfloat* ParticleEmitter::particlePositionData = new GLfloat[4 * PARTICLE_COUNT];
GLubyte* ParticleEmitter::particleColourData = new GLubyte[4 * PARTICLE_COUNT];

ParticleEmitter::ParticleEmitter(){

	previousParticle = textureIndex = 0;
	offset = vec2(RandomFloat(-0.2f, 0.2f), RandomFloat(-0.2f, 0.2f));
	shader = NULL;
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

	for (int i = 0; i < previousParticle; i++) {
		
		if (particleList[i].life < 0) {
			
			previousParticle = i;
			
			return i;
		}
	}

	/* All particles are taken, override the first one */
	return 0;
}

void ParticleEmitter::LoadParticles(int mode) {
	
	/* Background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	/* Enable depth test */
	glEnable(GL_DEPTH_TEST);
	/* Accept fragment if it closer to the camera than the previous one */
	glDepthFunc(GL_LESS);

	/* Create and compile the GLSL programs from the shaders */
	shader = new Shader(VERTEX_SHADER, FRAGMENT_SHADER);
	particleShaderProgramID = shader->CompileShaders();

	shader = new Shader(TEXTURE_ATLAS_VERTEX_SHADER, TEXTURE_ATLAS_FRAGMENT_SHADER);
	fireShaderProgramID = shader->CompileShaders();

	numOfRowsID = glGetUniformLocation(fireShaderProgramID, "numOfRows");
	offsetID = glGetUniformLocation(fireShaderProgramID, "offset");
	cameraRightID = glGetUniformLocation(particleShaderProgramID, "cameraRight");
	cameraUpID = glGetUniformLocation(particleShaderProgramID, "cameraUp");
	viewProjMatrixID = glGetUniformLocation(particleShaderProgramID, "viewProjectionMatrix");
	orientationID = glGetUniformLocation(particleShaderProgramID, "orientation");
	textureID = glGetUniformLocation(particleShaderProgramID, "myTexture");

	/* Load textures */
	snowflakeTexture = texture.LoadDDSTexture(SNOWFLAKE_TEXTURE);
	raindropTexture = texture.LoadDDSTexture(RAINDROP_TEXTURE);
	starTexture = texture.LoadDDSTexture(STAR_TEXTURE);
	cloudTexture = texture.LoadDDSTexture(CLOUD_TEXTURE);
	fireTexture = texture.LoadDDSTexture(FIRE_TEXTURE);

	for (int i = 0; i < PARTICLE_COUNT; i++) {
		
		particleList[i].life = -1.0f;
		particleList[i].cameraDistance = -1.0f;
	}

	/* The VBO containing the 4 vertices of the particles that will be shared by all particles */
	static const GLfloat vertexBufferData[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		-0.5f,  0.5f,
		0.5f,  0.5f
	};

	/* The VBO containing the vertices of the particles */
	vertexBuffer = VAO;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	/* The VBO containing the positions and sizes of the particles */
	positionBuffer = VAO;
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * PARTICLE_COUNT * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	/* The VBO containing the colours of the particles */
	colourBuffer = VAO;
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * PARTICLE_COUNT * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	lastTime = glfwGetTime();
}

void ParticleEmitter::DrawParticles(GLFWwindow* window, int mode, float gravity, bool pause) {
		
	/* Clear the screen */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double currentTime = glfwGetTime();
	double age = currentTime - lastTime;
	lastTime = currentTime;

	/* Move the camera given keyboard and mouse inputs */
	camera.CameraInputs(window);

	mat4 projectionMatrix = camera.getProjectionMatrix();
	mat4 viewMatrix = camera.getViewMatrix();
	mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		
	float spread;
	vec3 mainDirection, randomDirection;

	/* Generate a new particle */
	int newParticle;
		
	if (!pause) {
			
		newParticle = (int)(age*1000.0f);
	}
	else {

		newParticle = 0;
	}

	if (newParticle > (int)(0.016f*1000.0f)) {
			
		newParticle = (int)(0.016f*1000.0f);
	}

	for (int i = 0; i < newParticle; i++) {
		
		int index = FindNextParticle(mode);
		vec3 deltaPosition = camera.getDeltaPosition();

		if (mode == SNOW_DEMO) {
				
			spread = 1.5f;
			mainDirection = vec3(0.0f, -10.0f, 0.0f);
			randomDirection = vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f)); 
			
			particleList[index].life = 5.0f; 
			particleList[index].position = vec3(RandomFloat(-20.0f + deltaPosition.x, 20.0f + deltaPosition.x), RandomFloat(18.0f + deltaPosition.y, 23.0f + deltaPosition.y), RandomFloat(-20.0f + deltaPosition.z, 20.0f + deltaPosition.z));
			particleList[index].size = RandomFloat(0.1f, 0.7f); 
			particleList[index].red = 255.0f;
			particleList[index].green = 255.0f;
			particleList[index].blue = 255.0f;
			particleList[index].alpha = (RandomFloat(0.0f, 255.0f)) / 3;
		}

		else if (mode == RAIN_DEMO) {

			spread = 1.5f;
			mainDirection = vec3(0.0f, -15.0f, 0.0f);
			randomDirection = vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f));
			
			particleList[index].life = 5.0f; 
			particleList[index].position = vec3(RandomFloat(-20.0f + deltaPosition.x, 20.0f + deltaPosition.x), RandomFloat(18.0f + deltaPosition.y, 23.0f + deltaPosition.y), RandomFloat(-20.0f + deltaPosition.z, 20.0f + deltaPosition.z));
			particleList[index].size = RandomFloat(0.01f, 0.2f); 
			particleList[index].red = 50.0f;
			particleList[index].green = 200.0f;
			particleList[index].blue = 255.0f;
			particleList[index].alpha = (RandomFloat(0.0f, 255.0f)) / 3;
		}

		else if (mode == STARS_DEMO) {
				
			spread = 0.0f;
			mainDirection = vec3(0.0f, 0.0f, 15.0f);
			randomDirection = vec3(0.0f, 0.0f, 0.0f); 
			
			particleList[index].life = 5.0f; 
			particleList[index].position = vec3(RandomFloat(-15.0f + deltaPosition.x, 15.0f + deltaPosition.x), RandomFloat(-15.0f + deltaPosition.y, 15.0f + deltaPosition.y), RandomFloat(-25.0f + deltaPosition.z, -20.0f + deltaPosition.z));
			particleList[index].size = RandomFloat(0.08f, 0.5f); 
			particleList[index].red = 255.0f;
			particleList[index].green = 255.0f;
			particleList[index].blue = RandomFloat(0.0f, 255.0f);
			particleList[index].alpha = (RandomFloat(0.0f, 255.0f)) / 3;
		}
			
		else if (mode == CLOUD_DEMO) {

			spread = 0.0f;
			mainDirection = vec3(1.0f, 0.0f, 0.0f);
			randomDirection = vec3(0.0f, 0.0f, 0.0f);

			particleList[index].life = RandomFloat(6.0f, 7.0f); 
			particleList[index].position = vec3(-5.0f, RandomFloat(-0.5f + offset.y, 0.5f + offset.y), RandomFloat(-4.5f + offset.x, -3.5f + offset.x));
			particleList[index].size = RandomFloat(0.1f, 0.7f); 
			particleList[index].red = 255;
			particleList[index].green = 255;
			particleList[index].blue = 255;
			particleList[index].alpha = (RandomFloat(215.0f, 255.0f)) / 3;
		}

		else if (mode == FOUNTAIN_DEMO) {
				
			spread = 1.5f;
			mainDirection = vec3(0.0f, 4.0f, 0.0f);
			randomDirection = vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f)); 
			
			particleList[index].life = 5.0f; 
			particleList[index].position = vec3(0.0f, 0.0f, -2.0f);
			particleList[index].size = RandomFloat(0.003f, 0.03f); 
			particleList[index].red = 50.0f;
			particleList[index].green = 200.0f;
			particleList[index].blue = 255.0f;
			particleList[index].alpha = (RandomFloat(0.0f, 255.0f)) / 3;
		}

		else if (mode == SMOKE_DEMO) {
				
			spread = RandomFloat(0.005f, 0.035f);
			mainDirection = vec3(RandomFloat(-0.4f, 0.4f), 2.0f, RandomFloat(-0.4f, 0.4f));
			randomDirection = vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f));
			
			particleList[index].life = RandomFloat(7.0f, 8.0f); 
			particleList[index].position = vec3(RandomFloat(-0.2f + offset.x, 0.2f + offset.x), 0.0f, RandomFloat(-10.2f + offset.y, -9.8f + offset.y));
			particleList[index].size = RandomFloat(0.1f, 0.3f); 
			particleList[index].red = 40.0f;
			particleList[index].green = 40.0f;
			particleList[index].blue = 40.0f;
			particleList[index].alpha = RandomFloat(215.0f, 255.0f) / 3;
		}

		else if (mode == FIRE_DEMO) {
				
			spread = 0.0f;
			mainDirection = vec3(0.0f, 0.0f, 0.0f);
			randomDirection = vec3(0.0f, 0.0f, 0.0f);
			
			particleList[index].life = 0.6f; 
			particleList[index].position = vec3(0.0f, 0.0f, -5.0f);
			particleList[index].size = 1.4f; 
			particleList[index].red = 0.0f;
			particleList[index].green = 0.0f;
			particleList[index].blue = 0.0f;
			particleList[index].alpha = 255.0f;
		}

		particleList[index].speed = mainDirection + randomDirection*spread;
	}

	/* Simulate all particles */
	int particleIndex = 0;

	for (int i = 0; i < PARTICLE_COUNT; i++) {

		Particle& particle = particleList[i];

		if (particle.life > 0.0f) {

			/* Decrease life */
			if (!pause) {
				
				particle.life -= age;
			}

			if (particle.life > 0.0f) {

				if(!pause) {
					
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

						if (particle.position.z <= -2.8f && particle.position.z >= -5.8f) {
							
							offset.x += RandomFloat(-0.0001f, 0.0001f);
						}

						if (particle.position.y <= 1.8f && particle.position.y >= -1.8f) {
							
							offset.y += RandomFloat(-0.0001f, 0.0001f);
						}

						if (particle.life <= 1.0f) {

							particle.alpha /= ((RandomFloat(0.005f, 0.01f) / 2) + 1.0f);
						}
					}

					else if (mode == FOUNTAIN_DEMO) {
						
						particle.speed += vec3(0.0f, gravity, 0.0f) * (float)age * 0.5f;
					}

					else if (mode == SMOKE_DEMO) {

						if ((int)currentTime % 5 == 0) {

							windEffect.x = (RandomFloat(-0.6f, 0.6f));
							windEffect.z = (RandomFloat(-0.6f, 0.6f));
						}

						if (particle.position.x >= -0.1f && particle.position.x <= 0.1f) {
							offset.x += RandomFloat(-0.001f, 0.001f);
						}

						if (particle.position.z >= -10.1f && particle.position.z <= 9.9f) {
							offset.y += RandomFloat(-0.001f, 0.001f);
						}
					
							particle.speed += vec3(windEffect.x, 0.0f, windEffect.z) * (float)age * 0.5f;
							particle.size += 0.004f;

						if (particle.life <= 6.0f) {

							particle.alpha /= ((RandomFloat(0.000005f, 0.00001f) / 2) + 1.0f);
						}
					}

					else if (mode == FIRE_DEMO) {
						particle.speed += vec3(0.0f, 0.0f, 0.0f) * (float)age * 0.5f;
					}

						particle.position += particle.speed * (float)age;
				}
				
				particle.cameraDistance = length2(particle.position - camera.getPosition());

				/* Fill the GPU buffer */
				particlePositionData[4 * particleIndex + 0] = particle.position.x;
				particlePositionData[4 * particleIndex + 1] = particle.position.y;
				particlePositionData[4 * particleIndex + 2] = particle.position.z;
				particlePositionData[4 * particleIndex + 3] = particle.size;

				particleColourData[4 * particleIndex + 0] = particle.red;
				particleColourData[4 * particleIndex + 1] = particle.green;
				particleColourData[4 * particleIndex + 2] = particle.blue;
				particleColourData[4 * particleIndex + 3] = particle.alpha;
			}

			else {
				/* Particles that just died will be put at the end of the buffer */
				particle.cameraDistance = -1.0f;
			}
			/* Go to next particle */
			particleIndex++;
		}
	}

	if (!pause && mode == FIRE_DEMO) {
		
		AnimateTexture(0.6f, age, 16);
	}

	/* Sort the particles */
	sort(&particleList[0], &particleList[PARTICLE_COUNT]);

	/* Buffer orphaning */
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * PARTICLE_COUNT * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * particleIndex * sizeof(GLfloat), particlePositionData);

	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * PARTICLE_COUNT * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * particleIndex * sizeof(GLubyte), particleColourData);

	/* Enable blending */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Use the shader */
	if (mode == FIRE_DEMO) {
		
		glUseProgram(fireShaderProgramID);
	}
	else {
		
		glUseProgram(particleShaderProgramID);
	}

	/* Bind the textures */
	glActiveTexture(GL_TEXTURE0);
	
	if (mode == SNOW_DEMO) {
		
		glBindTexture(GL_TEXTURE_2D, snowflakeTexture);
	}
	else if (mode == RAIN_DEMO || mode == FOUNTAIN_DEMO) {
		
		glBindTexture(GL_TEXTURE_2D, raindropTexture);
	}
	else if (mode == STARS_DEMO) {
		
		glBindTexture(GL_TEXTURE_2D, starTexture);
	}
	else if (mode == CLOUD_DEMO || mode == SMOKE_DEMO) {
		
		glBindTexture(GL_TEXTURE_2D, cloudTexture);
	}
	else if (mode == FIRE_DEMO) {
		
		glBindTexture(GL_TEXTURE_2D, fireTexture);
	}

	/* Uniform variables */
	glUniform1i(textureID, 0);
	glUniform3f(cameraRightID, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	glUniform3f(cameraUpID, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
	glUniformMatrix4fv(viewProjMatrixID, 1, GL_FALSE, &viewProjectionMatrix[0][0]);
	glUniformMatrix4fv(orientationID, 1, GL_FALSE, &orientation[0][0]);

	/* First attribute buffer - vertices */
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Second attribute buffer - positions of particle centres */
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Third attribute buffer - particle colours */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	/* Particle vertices - always reuse the same 4 vertices */
	glVertexAttribDivisor(0, 0);
	/* Positions - one per quad (centre) */
	glVertexAttribDivisor(1, 1);
	/* Colour - one per quad */
	glVertexAttribDivisor(2, 1);
	/* Draw the quads */
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleIndex);
	/* Disable the vertex attributes */
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void ParticleEmitter::CleanUp(GLFWwindow* window) {

	/* Cleanup VBO and shader, cleanup steps are generally performed in reverse order to setup steps */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &colourBuffer);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	delete[] particlePositionData;
	glDeleteTextures(1, &textureID);
	glDeleteProgram(particleShaderProgramID);
	glDeleteProgram(fireShaderProgramID);
}

void ParticleEmitter::TextureAtlas(int spriteIndex, const int numOfRows, const int numOfColumns) {
	
	int col = spriteIndex % numOfColumns;
	int row = numOfRows - (spriteIndex / numOfColumns) - 1;
	
	float x = (float)col / (float)numOfColumns;
	float y = (float)row / (float)numOfRows;
	
	/* Use the shader */
	glUseProgram(fireShaderProgramID);
	glUniform2f(offsetID, x, y);
	glUniform1f(numOfRowsID, numOfRows);
}

void ParticleEmitter::AnimateTexture(float maxAge, double age, int numOfTextures) {

	static double timer = 0.0;
	timer += age;

	if (timer > maxAge || textureIndex >= numOfTextures) {
		
		timer -= maxAge;
		textureIndex = 0;
	}
	if (timer > (((textureIndex + 1) * maxAge) / numOfTextures) && textureIndex < numOfTextures) {
		
		TextureAtlas(textureIndex, 4, 4);
		textureIndex++;
	}
}
