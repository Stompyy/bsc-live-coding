#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <string>

GLuint loadTextureFromFile(const std::string& filename);