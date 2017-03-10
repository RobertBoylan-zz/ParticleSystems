#pragma once
#include "Globals.h"

#define VERTEX_SHADER "C:/Users/Robert/Documents/Visual Studio 2015/Projects/ParticleSystems/ParticleSystems/Shaders/VertexShader.txt"
#define FRAGMENT_SHADER "C:/Users/Robert/Documents/Visual Studio 2015/Projects/ParticleSystems/ParticleSystems/Shaders/FragmentShader.txt"
#define TEXTURE_ATLAS_VERTEX_SHADER "C:/Users/Robert/Documents/Visual Studio 2015/Projects/ParticleSystems/ParticleSystems/Shaders/TextureAtlasVertexShader.txt"
#define TEXTURE_ATLAS_FRAGMENT_SHADER "C:/Users/Robert/Documents/Visual Studio 2015/Projects/ParticleSystems/ParticleSystems/Shaders/TextureAtlasFragmentShader.txt"

class Shader {

public:
	Shader(const char* _vertFileName, const char* _fragFileName);

	/* Reads the shader files */
	char* ReadShaderSource(const char* shaderFile);
	/* Adds the shader object to the program object */
    	void AddShader(GLuint shaderProgram, const char* shaderText, GLenum shaderType);
	/* Compiles the shader files */
	GLuint CompileShaders();

private: 
	const char* vertFileName;
	const char* fragFileName;
};
