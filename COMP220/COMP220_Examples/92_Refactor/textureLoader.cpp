#include "textureLoader.h"

GLuint loadTextureFromFilename(const std::string& filename)
{
	GLuint textureID;

	GLenum	textureFormat = GL_RGB;
	GLenum	internalFormat = GL_RGB8;

	SDL_Surface * surface = IMG_Load(filename.c_str());
	if (surface == nullptr)
	{
		printf("Could not load file %s", IMG_GetError());
		return 0;
	}

	GLint	nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4)					//	contains	an	alpha	channel
	{
		if (surface->format->Rmask == 0x000000ff) {
			textureFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
		}
		else {
			textureFormat = GL_BGRA;
			internalFormat = GL_RGBA8;
		}
	}
	else if (nOfColors == 3)					//	no	alpha	channel
	{
		if (surface->format->Rmask == 0x000000ff) {
			textureFormat = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else {
			textureFormat = GL_BGR;
			internalFormat = GL_RGB8;
		}
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

	SDL_FreeSurface(surface);

	return textureID;
}

TextureLoader::TextureLoader()
{
	// Ensures empty starting value
	m_TextureMap.clear();
}
void TextureLoader::init(std::vector<std::string> textureNamesToLoad)
{

	// Builds the map of GLuint textureIDs and their identifying string keys
	for (std::string textureName : textureNamesToLoad)
	{
		m_TextureMap[textureName] = loadTextureFromFilename(textureName);
	}
}

TextureLoader::~TextureLoader()
{
	destroy();
}

void TextureLoader::destroy()
{
}
