#pragma once
#include "Globals.h"

#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

#define SNOWFLAKE_TEXTURE "C:/Users/Robert/Documents/Visual Studio 2015/Projects/ParticleSystems/ParticleSystems/Textures/Snowflake.dds"
#define STAR_TEXTURE "C:/Users/Robert/Documents/Visual Studio 2015/Projects/ParticleSystems/ParticleSystems/Textures/Star.dds"
#define RAINDROP_TEXTURE "C:/Users/Robert/Documents/Visual Studio 2015/Projects/ParticleSystems/ParticleSystems/Textures/Raindrop.dds"
#define CLOUD_TEXTURE "C:/Users/Robert/Documents/Visual Studio 2015/Projects/ParticleSystems/ParticleSystems/Textures/Cloud.dds"
#define FIRE_TEXTURE "C:/Users/Robert/Documents/Visual Studio 2015/Projects/ParticleSystems/ParticleSystems/Textures/Fire.dds"

class Texture {

public:
	Texture();

	/* Load the .dds texture file */
	GLuint LoadDDSTexture(const char* fileName);

private:
	unsigned int width, height;
};
