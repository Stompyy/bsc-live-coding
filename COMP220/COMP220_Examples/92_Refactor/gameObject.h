#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "model.h"
#include "texture.h"
#include "shader.h"

#include "Transform.h"
#include "Material.h"
#include "Physics.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	Transform* transform;
	Material* material;
	Physics* physics;

	glm::mat4& getModelMatrix() { return m_ModelMatrix; }

	GLuint getShaderProgramID() { return m_shaderProgramID; }
	void UpdateTransformOrigin() { physics->getTransform().setOrigin(btVector3(transform->getPosition().x, transform->getPosition().y, transform->getPosition().z)); }

	void loadMesh(const std::string& filename);
	void loadDiffuseMap(const std::string& filename);
	void loadShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void destroy();
	
	void update();
	void preRender();
	void render();

private:
	std::vector<Mesh*> m_Meshes;

	glm::mat4 m_ModelMatrix;

	GLuint m_DiffuseMapID;
	GLuint m_shaderProgramID;
};
