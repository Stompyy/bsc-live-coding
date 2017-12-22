#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <string>
#include <GL\glew.h>
#include <SDL_opengl.h>

#include <vector>
#include "vertex.h"
#include "Mesh.h"

//VBO is Vertex Buffer Object EBO is Element Buffer Object
//bool loadModelFromFile(const std::string& filename, GLuint VBO, GLuint EBO, unsigned int& numberOfVerts, unsigned int& numberOfIndices);

bool oldLoadMeshFromFile(const std::string& filename, std::vector<Mesh*>& meshes);	// Memory address of the vector of pointers?!
std::vector<Mesh*>& loadMeshFromFile(const std::string& filename);
