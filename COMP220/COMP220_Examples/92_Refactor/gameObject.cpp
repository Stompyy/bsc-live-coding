#include "gameObject.h"

GameObject::GameObject()
{
	transform = new Transform();
	material = new Material();
	physics = new Physics();
	shaderUniforms = new ShaderUniforms();

	// Ensures initial value
	m_Meshes.clear();
	m_DiffuseMapID = 0;

	m_ModelMatrix = glm::mat4(1.0f);

	material->setAmbientColour(0.4f, 0.4f, 0.4f);
	material->setDiffuseColour(0.6f, 0.6f, 0.6f);
	material->setSpecularColour(1.0f, 1.0f, 1.0f);

	GLuint shaderProgramID = 0;
}


GameObject::~GameObject()
{
}

void GameObject::init(
	const std::vector<Mesh*> meshes,
	const GLuint textureID,
	const GLuint shaderID,
	const glm::vec3 initialPosition,
	const float mass,
	const btVector3 collisionSize)
{
	m_Meshes = meshes;
	m_DiffuseMapID = textureID;
	m_ShaderProgramID = shaderID;
	shaderUniforms->init(m_ShaderProgramID);
	transform->setPosition(initialPosition);
	physics->setMass(mass);
	physics->setCollisionShapeSize(collisionSize);
	physics->getTransform().setIdentity();
	// Set the physics position to the gameObject's transform
	physics->getTransform().setOrigin(btVector3(transform->getPosition().x, transform->getPosition().y, transform->getPosition().z));
	physics->updateMotionState();
}

void GameObject::destroy()
{
	glDeleteTextures(1, &m_DiffuseMapID);
	glDeleteProgram(m_ShaderProgramID);

	if (m_Meshes.size() > 0)
	{
		auto iter = m_Meshes.begin();
		while (iter != m_Meshes.end())
		{
			if ((*iter))
			{
				(*iter)->destroy();
				delete (*iter);
				iter = m_Meshes.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
	delete shaderUniforms;
	delete physics;
	delete material;
	delete transform;
}

void GameObject::update()
{

	if (physics->getRigidBody() != nullptr)
	{
		transform->setPosition(
			physics->getTransform().getOrigin().getX(), 
			physics->getTransform().getOrigin().getY(), 
			physics->getTransform().getOrigin().getZ());
	}

	// Update the model matrix
	glm::mat4 translationMatrix = glm::translate(transform->getPosition());
	glm::mat4 scaleMatrix = glm::scale(transform->getScale());
	glm::mat4 rotationMatrix = glm::toMat4(transform->getRotation());

	// Remember TRS order
	m_ModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void GameObject::preRender(Camera* camera, Light* light)
{
	glUseProgram(m_ShaderProgramID);

	// Activate the texture
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, m_DiffuseMapID);

	// Send the values to the shaders
	shaderUniforms->update(camera, light, material, &m_ModelMatrix);
}

void GameObject::render()
{
	for (Mesh* currentMesh : m_Meshes)
	{
		currentMesh->render();
	}
}