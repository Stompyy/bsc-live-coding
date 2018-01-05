#include "GameObject.h"

GameObject::GameObject()
{
	m_Transform = new Transform();
	m_Material = new Material();
	m_Physics = new Physics();
	m_ShaderUniforms = new ShaderUniforms();

	// Ensures initial value
	m_Meshes.clear();
	m_DiffuseMapID = 0;

	m_Material->setAmbientColour(0.4f, 0.4f, 0.4f);
	m_Material->setDiffuseColour(0.6f, 0.6f, 0.6f);
	m_Material->setSpecularColour(1.0f, 1.0f, 1.0f);

	m_ShaderProgramID = 0;
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
	m_ShaderUniforms->init(m_ShaderProgramID);
	m_Transform->setPosition(initialPosition);
	m_Physics->setMass(mass);
	m_Physics->setCollisionShapeSize(collisionSize);
	m_Physics->getTransform().setIdentity();
	// Set the physics position to the gameObject's transform
	m_Physics->getTransform().setOrigin(btVector3(m_Transform->getPosition().x, m_Transform->getPosition().y, m_Transform->getPosition().z));
	m_Physics->createRigidBody();
	m_Physics->getRigidBody()->setUserPointer(this);
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
	delete m_ShaderUniforms;
	delete m_Physics;
	delete m_Material;
	delete m_Transform;
}

void GameObject::update()
{

	if (m_Physics->getRigidBody() != nullptr)
	{
		m_Transform->setPosition(
			m_Physics->getTransform().getOrigin().getX(), 
			m_Physics->getTransform().getOrigin().getY(), 
			m_Physics->getTransform().getOrigin().getZ());

		m_Transform->update();
	}
}

void GameObject::preRender(Camera* camera, Light* lightOne, Light* lightTwo)
{
	glUseProgram(m_ShaderProgramID);

	// Activate the texture
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, m_DiffuseMapID);

	// Send the values to the shaders
	m_ShaderUniforms->update(camera, lightOne, lightTwo, m_Material, m_Transform);
}

void GameObject::render()
{
	for (Mesh* currentMesh : m_Meshes)
	{
		currentMesh->render();
	}
}