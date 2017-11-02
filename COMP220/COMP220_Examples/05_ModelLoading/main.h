#pragma once

#include <string>
#include <vector>
#include <fstream> 

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "texture.h"
#include "shader.h"

#include "model.h"

using namespace glm;

/*
copy "..\..\..\Libraries\SDL2-2.0.6\lib\x64\SDL2.dll" "$(OutDir)\SDL2.dll"
copy "..\..\..\Libraries\glew-2.1.0\bin\Release\x64\glew32.dll" "$(OutDir)\glew32.dll"
*/