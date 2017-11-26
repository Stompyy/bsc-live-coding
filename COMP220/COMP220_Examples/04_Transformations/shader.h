#pragma once

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <string>
#include <vector>
#include <fstream> 

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
