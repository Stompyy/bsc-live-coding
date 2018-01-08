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
	destroy();
}

void GameObject::init(
	const std::string identifyingName,
	const std::vector<Mesh*> meshes,
	const GLuint textureID,
	const GLuint shaderID,
	Transform* initialTransform,
	const float mass,
	const btVector3 collisionSize,
	const bool forceActive)
{
	m_Meshes = meshes;
	m_DiffuseMapID = textureID;
	m_ShaderProgramID = shaderID;
	m_ShaderUniforms->init(m_ShaderProgramID);
	m_Transform = initialTransform;
	m_Physics->setMass(mass);
	m_Physics->setCollisionShapeSize(collisionSize);
	m_Physics->getTransform().setIdentity();
	m_Physics->setIsPhysicsActive(forceActive);

	// Set the physics transform to the gameObject's transform
	m_Physics->getTransform().setOrigin(btVector3(m_Transform->getPosition().x, m_Transform->getPosition().y, m_Transform->getPosition().z));
	m_Physics->getTransform().setRotation(btQuaternion(m_Transform->getRotation().w, m_Transform->getRotation().x, m_Transform->getRotation().y, m_Transform->getRotation().z));
	m_Physics->createRigidBody();
	m_Physics->getRigidBody()->setUserPointer(this);
}

void GameObject::destroy()
{
	glDeleteTextures(1, &m_DiffuseMapID);
	glDeleteProgram(m_ShaderProgramID);
}

void GameObject::update()
{
	// Update the physics transform to the rigidbody position, and activate if isPhysicsActive is true
	m_Physics->update();

	// Update gameObject position to the physics position
	if (m_Physics->getRigidBody() != nullptr)
	{
		m_Transform->setPosition(
			m_Physics->getTransform().getOrigin().getX(), 
			m_Physics->getTransform().getOrigin().getY(), 
			m_Physics->getTransform().getOrigin().getZ());

		// Update the model matrix
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

void GameObject::rotateInPosition()
{
	m_Transform->updateRotatingModel();
	m_Physics->getTransform().setRotation(btQuaternion(
		m_Transform->getRotation().w,
		m_Transform->getRotation().x,
		m_Transform->getRotation().y,
		m_Transform->getRotation().z
	));
}
