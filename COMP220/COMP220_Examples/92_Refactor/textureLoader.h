#pragma once

#include <string>
#include <vector>
#include <map>
#include <GL\glew.h>
//#include <SDL_opengl.h>
#include <SDL_image.h>
//#include "model.h"

class TextureLoader
{
public:
	TextureLoader(std::vector<std::string> textureFileNames);
	~TextureLoader();

	// Error checking here if nullptr
	GLuint getTextureID(const std::string& textureName) { return m_TextureMap[textureName]; }

private:
	// Create an empty texture map. Aim to only load in each texture once, but able to reuse.
	std::map<std::string, GLuint> m_TextureMap;
};

