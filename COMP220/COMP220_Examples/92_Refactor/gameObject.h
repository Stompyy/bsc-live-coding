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

	void init(
		const std::vector<Mesh*> meshes, 
		const GLuint textureID, 
		const std::string& vertexShaderFilename, 
		const std::string& fragmentShaderFilename, 
		const glm::vec3 initialPosition, 
		const float mass,
		const btVector3 collisionSize);

	Transform* transform;
	Material* material;
	Physics* physics;

	glm::mat4& getModelMatrix() { return m_ModelMatrix; }

	GLuint getShaderProgramID() { return m_shaderProgramID; }
	void updateTransformOrigin() { physics->getTransform().setOrigin(btVector3(transform->getPosition().x, transform->getPosition().y, transform->getPosition().z)); }

	void loadMesh(const std::string& filename);
	void setMesh(const std::vector<Mesh*>& meshes) { m_Meshes = meshes; }

	//void loadDiffuseMap(const std::string& filename);
	void setDiffuseMap(const GLuint textureID) { m_DiffuseMapID = textureID; }
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

