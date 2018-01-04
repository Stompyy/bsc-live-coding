#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

#include "Mesh.h"
#include "shader.h"

#include "Transform.h"
#include "Material.h"
#include "Physics.h"
#include "ShaderUniforms.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	// Tidier initialisation of class than by setting individually in main.cpp
	void init(
		const std::vector<Mesh*> meshes,
		const GLuint textureID,
		const GLuint shaderID,
		const glm::vec3 initialPosition,
		const float mass,
		const btVector3 collisionSize);

	Transform* transform;
	Material* material;
	Physics* physics;
	ShaderUniforms* shaderUniforms;

	void destroy();
	void update();

	// Update the shaders with the latest values
	void preRender(Camera* camera, Light* light);

	// Draw
	void render();

private:
	std::vector<Mesh*> m_Meshes;

	glm::mat4 m_ModelMatrix;

	GLuint m_DiffuseMapID;
	GLuint m_ShaderProgramID;

	glm::vec3 m_Up;
};
