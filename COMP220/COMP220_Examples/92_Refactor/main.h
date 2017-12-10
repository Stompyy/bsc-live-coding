#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream> 

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <btBulletDynamicsCommon.h>

#include "texture.h"
#include "shader.h"

#include "vertex.h"
#include "model.h"
#include "gameObject.h"
#include "Player.h"
#include "Camera.h"
#include "Light.h"
#include "PhysicsEngine.h"
#include "PostProcessing.h"