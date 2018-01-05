#pragma once

#include <string>
#include <vector>
#include <map>
#include <GL\glew.h>
#include <SDL_image.h>

class TextureLoader
	// Preload in all textures needed so as to be able to reuse, rather than just reload everytime
	// Stores the GLuint in a map, where each key is the texture's filename i.e. string trexColour.png : Gluint trexColour.png
{
public:
	TextureLoader();
	~TextureLoader();

	void init(std::vector<std::string> textureFileNames);

	// Error checking here if nullptr
	GLuint getTextureID(const std::string& textureName) { return m_TextureMap[textureName]; }

	void destroy();

private:
	// Create an empty texture map. Aim to only load in each texture once, but able to reuse.
	std::map<std::string, GLuint> m_TextureMap;
};

