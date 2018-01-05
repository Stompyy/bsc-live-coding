#pragma once

#include <vector>
#include <string>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <assimp\Importer.hpp>

#include "vertex.h"

// Trying to read texture values from mesh import... wip
struct FBXTexture {
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CopyBufferData(Vertex* verts, unsigned int numberOfVerts, unsigned int* indices, unsigned int numberOfIndices);

	void init();
	//This renders a single mesh
	void render();
	void destroy();

private:
	GLuint m_VBO;	// Vertex buffer object
	GLuint m_EBO;	// Element buffer object
	GLuint m_VAO;	// Vertex array object

	unsigned int m_NumberOfVerts;
	unsigned int m_NumberOfIndices;
	unsigned int m_NumberOfTextures;
};

