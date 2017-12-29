#include "gameObject.h"

GameObject::GameObject()
{
	transform = new Transform();
	material = new Material();
	physics = new Physics();

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
	m_shaderProgramID = LoadShaders(vertexShaderFilename.c_str(), fragmentShaderFilename.c_str());
	transform->setPosition(initialPosition);
	physics->setMass(mass);
	physics->setCollisionShapeSize(collisionSize);
	physics->getTransform().setIdentity();
	updateTransformOrigin();
	physics->updateMotionState();
}

void GameObject::loadMesh(const std::string& filename)
{
	// Include skeleton stuff here too
	oldLoadMeshFromFile(filename, m_Meshes);
}

/*
void GameObject::loadDiffuseMap(const std::string & filename)
{
	m_DiffuseMapID = loadTextureFromFile(filename);
}
*/

void GameObject::loadShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	m_shaderProgramID = LoadShaders(vertexShaderFilename.c_str(), fragmentShaderFilename.c_str());
}

void GameObject::destroy()
{
	glDeleteTextures(1, &m_DiffuseMapID);
	glDeleteProgram(m_shaderProgramID);

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
	glm::mat4 rotationMatrix = 
		  glm::rotate(transform->getRotation().x, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(transform->getRotation().y, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(transform->getRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

	// TRS
	m_ModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	

}

void GameObject::preRender()
{

	glUseProgram(m_shaderProgramID);
	// Activate the texture
	glActiveTexture(GL_TEXTURE0);
	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, m_DiffuseMapID);

	GLint modelMatrixLocation = glGetUniformLocation(m_shaderProgramID, "modelMatrix");
	GLint textureLocation = glGetUniformLocation(m_shaderProgramID, "baseTexture");
	
	GLint ambientMaterialColourLocation = glGetUniformLocation(m_shaderProgramID, "ambientMaterialColour");
	GLint diffuseMaterialColourLocation = glGetUniformLocation(m_shaderProgramID, "diffuseMaterialColour");
	GLint specularMaterialColourLocation = glGetUniformLocation(m_shaderProgramID, "specularMaterialColour");
	GLint specularPowerLocation = glGetUniformLocation(m_shaderProgramID, "specularPower");

	// Bad getting every frame. store as a map in initialising, string on right, GLint on left

	// Send everything across
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(m_ModelMatrix));		
	glUniform1i(textureLocation, 0);
	glUniform4fv(ambientMaterialColourLocation, 1, value_ptr(material->getAmbientColour()));
	glUniform4fv(diffuseMaterialColourLocation, 1, value_ptr(material->getDiffuseColour()));
	glUniform4fv(specularMaterialColourLocation, 1, value_ptr(material->getSpecularColour()));
	glUniform1f(specularPowerLocation, material->getSpecularPower());
}

void GameObject::render()
{
	for (Mesh* currentMesh : m_Meshes)
	{
		currentMesh->render();
	}
}
