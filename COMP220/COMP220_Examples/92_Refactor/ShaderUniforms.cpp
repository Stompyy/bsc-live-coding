#include "ShaderUniforms.h"



ShaderUniforms::ShaderUniforms()
{
	// Default zero values
	m_TextureLocation = 0;
	m_ModelMatrixLocation = 0;
	m_ViewMatrixLocation = 0;
	m_ProjectionMatrixLocation = 0;
	m_CameraPositionLocation = 0;
	m_LightOneDirectionLocation = 0;
	m_AmbientLightOneColourLocation = 0;
	m_DiffuseLightOneColourLocation = 0;
	m_SpecularLightOneColourLocation = 0;
	m_LightTwoDirectionLocation = 0;
	m_AmbientLightTwoColourLocation = 0;
	m_DiffuseLightTwoColourLocation = 0;
	m_SpecularLightTwoColourLocation = 0;
	m_AmbientMaterialColourLocation = 0;
	m_DiffuseMaterialColourLocation = 0;
	m_SpecularMaterialColourLocation = 0;
	m_SpecularPowerLocation = 0;
}

ShaderUniforms::~ShaderUniforms()
{
}

void ShaderUniforms::init(GLuint shaderProgramID)
{
	// Find all the uniform locations. Only needed to do once per shaderProgramID
	m_TextureLocation = glGetUniformLocation(shaderProgramID, "baseTexture");

	m_ModelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	m_ViewMatrixLocation = glGetUniformLocation(shaderProgramID, "viewMatrix");
	m_ProjectionMatrixLocation = glGetUniformLocation(shaderProgramID, "projectionMatrix");

	m_CameraPositionLocation = glGetUniformLocation(shaderProgramID, "cameraPosition");

	// Light number one
	m_LightOneDirectionLocation = glGetUniformLocation(shaderProgramID, "lightOneDirection");
	m_AmbientLightOneColourLocation = glGetUniformLocation(shaderProgramID, "ambientLightOneColour");
	m_DiffuseLightOneColourLocation = glGetUniformLocation(shaderProgramID, "diffuseLightOneColour");
	m_SpecularLightOneColourLocation = glGetUniformLocation(shaderProgramID, "specularLightOneColour");

	// Light number two
	m_LightTwoDirectionLocation = glGetUniformLocation(shaderProgramID, "lightTwoDirection");
	m_AmbientLightTwoColourLocation = glGetUniformLocation(shaderProgramID, "ambientLightTwoColour");
	m_DiffuseLightTwoColourLocation = glGetUniformLocation(shaderProgramID, "diffuseLightTwoColour");
	m_SpecularLightTwoColourLocation = glGetUniformLocation(shaderProgramID, "specularLightTwoColour");

	m_AmbientMaterialColourLocation = glGetUniformLocation(shaderProgramID, "ambientMaterialColour");
	m_DiffuseMaterialColourLocation = glGetUniformLocation(shaderProgramID, "diffuseMaterialColour");
	m_SpecularMaterialColourLocation = glGetUniformLocation(shaderProgramID, "specularMaterialColour");

	m_SpecularPowerLocation = glGetUniformLocation(shaderProgramID, "specularPower");
}

void ShaderUniforms::update(Camera* camera, Light* lightOne, Light* lightTwo, Material* material, Transform* transform)
{
	// Send shader values
	// Always send vec4s instead of vec3s for efficiency, think about the 4x4 matrix multiplication, vec3s get ammended with a homogeneous w=0 for direction, or w=1 for position.

	// Texture
	glUniform1i(m_TextureLocation, 0);

	glm::mat4 tmpM = transform->getModelMatrix();
	glm::mat4 tmpV = camera->getViewMatrix();
	glm::mat4 tmpP = camera->getProjectionMatrix();

	// MVP
	glUniformMatrix4fv(m_ModelMatrixLocation, 1, GL_FALSE, value_ptr(transform->getModelMatrix()));
	glUniformMatrix4fv(m_ViewMatrixLocation, 1, GL_FALSE, value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(m_ProjectionMatrixLocation, 1, GL_FALSE, value_ptr(camera->getProjectionMatrix()));

	// Camera
	glUniform4fv(m_CameraPositionLocation, 1, value_ptr((glm::vec4)camera->getWorldLocation()->getPositionForShader()));

	// Light number one
	glUniform4fv(m_LightOneDirectionLocation, 1, value_ptr((glm::vec4)lightOne->getDirectionForShader()));
	glUniform4fv(m_AmbientLightOneColourLocation, 1, value_ptr(lightOne->getColour()->getAmbientColour()));
	glUniform4fv(m_DiffuseLightOneColourLocation, 1, value_ptr(lightOne->getColour()->getDiffuseColour()));
	glUniform4fv(m_SpecularLightOneColourLocation, 1, value_ptr(lightOne->getColour()->getSpecularColour()));

	// Light number two
	glUniform4fv(m_LightTwoDirectionLocation, 1, value_ptr((glm::vec4)lightTwo->getDirectionForShader()));
	glUniform4fv(m_AmbientLightTwoColourLocation, 1, value_ptr(lightTwo->getColour()->getAmbientColour()));
	glUniform4fv(m_DiffuseLightTwoColourLocation, 1, value_ptr(lightTwo->getColour()->getDiffuseColour()));
	glUniform4fv(m_SpecularLightTwoColourLocation, 1, value_ptr(lightTwo->getColour()->getSpecularColour()));

	// Material
	glUniform4fv(m_AmbientMaterialColourLocation, 1, value_ptr(material->getAmbientColour()));
	glUniform4fv(m_DiffuseMaterialColourLocation, 1, value_ptr(material->getDiffuseColour()));
	glUniform4fv(m_SpecularMaterialColourLocation, 1, value_ptr(material->getSpecularColour()));

	// Specular
	glUniform1f(m_SpecularPowerLocation, material->getSpecularPower());

}
