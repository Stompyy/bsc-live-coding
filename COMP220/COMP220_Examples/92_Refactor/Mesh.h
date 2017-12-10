#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <vector>

#include "vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CopyBufferData(Vertex* verts, unsigned int numberOfVerts, unsigned int* indices, unsigned int numberOfIndices);	// std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	void init();
	void render();
	void destroy();

private:
	GLuint m_VBO;	// Vertex buffer object
	GLuint m_EBO;	// Element buffer object
	GLuint m_VAO;	// Vertex array object
	
	unsigned int m_NumberOfVerts;
	unsigned int m_NumberOfIndices;

	

};

