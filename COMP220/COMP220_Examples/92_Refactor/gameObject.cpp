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
	const std::string& vertexShaderFilename,
	const std::string& fragmentShaderFilename,
	const glm::vec3 initialPosition,
	const float mass,
	const btVector3 collisionSize)
{
	m_Meshes = meshes;
	m_DiffuseMapID = textureID;
	m_ShaderProgramID = LoadShaders(vertexShaderFilename.c_str(), fragmentShaderFilename.c_str());
	shaderUniforms->init(m_ShaderProgramID);
	transform->setPosition(initialPosition);
	physics->setMass(mass);
	physics->setCollisionShapeSize(collisionSize);
	physics->getTransform().setIdentity();
	updateTransformOrigin();
	physics->updateMotionState();
}

void GameObject::loadShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	m_ShaderProgramID = LoadShaders(vertexShaderFilename.c_str(), fragmentShaderFilename.c_str());
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
}

void GameObject::update()
{

	if (physics->getRigidBody() != nullptr)
	{
		transform->setPosition(vec3(physics->getTransform().getOrigin().getX(), physics->getTransform().getOrigin().getY(), physics->getTransform().getOrigin().getZ()));
	}

	//if (Rigidbody)
	//{
	//	get pos rb
	//		update transform pos
	//}


	// Update the model matrix
	glm::mat4 translationMatrix = glm::translate(transform->getPosition());
	glm::mat4 scaleMatrix = glm::scale(transform->getScale());
	glm::mat4 rotationMatrix = glm::toMat4(transform->getRotation());

	// TRS
	m_ModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void GameObject::preRender(Camera* camera, Light* light)
{
	glUseProgram(m_ShaderProgramID);
	// Activate the texture
	glActiveTexture(GL_TEXTURE0);
	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, m_DiffuseMapID);

	shaderUniforms->update(camera, light, material, &m_ModelMatrix);
}

void GameObject::render()
{
	for (Mesh* currentMesh : m_Meshes)
	{
		currentMesh->render();
	}
}