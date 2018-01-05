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

	// Returns the Transform member
	Transform* getTransform() { return m_Transform; }

	// Returns the Material member
	Material* getMaterial() { return m_Material; }

	// Returns the Physics member
	Physics* getPhysics() { return m_Physics; }

	// Returns the Shader Uniform member
	ShaderUniforms* getShaderUniform() { return m_ShaderUniforms; }

	// Tidier initialisation of class than by setting individually in main.cpp
	void init(
		const std::vector<Mesh*> meshes,
		const GLuint textureID,
		const GLuint shaderID,
		const glm::vec3 initialPosition,
		const float mass,
		const btVector3 collisionSize);

	void destroy();
	void update();

	// Update the shaders with the latest values
	void preRender(Camera* camera, Light* lightOne, Light* lightTwo);

	// Draw
	void render();

private:
	Transform* m_Transform;
	Material* m_Material;
	Physics* m_Physics;
	ShaderUniforms* m_ShaderUniforms;

	std::vector<Mesh*> m_Meshes;

	GLuint m_DiffuseMapID;
	GLuint m_ShaderProgramID;
};