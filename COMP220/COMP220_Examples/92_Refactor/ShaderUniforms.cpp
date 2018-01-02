#include "ShaderUniforms.h"



ShaderUniforms::ShaderUniforms()
{
	m_ViewMatrixLocation = 0;
	m_ProjectionMatrixLocation = 0;
	m_CameraPositionLocation = 0;
	m_LightDirectionLocation = 0;
	m_AmbientLightColourLocation = 0;
	m_DiffuseLightColourLocation = 0;
	m_SpecularLightColourLocation = 0;
}


ShaderUniforms::~ShaderUniforms()
{
}

void ShaderUniforms::init(GLuint shaderProgramID)
{
	m_TextureLocation = glGetUniformLocation(shaderProgramID, "baseTexture");

	m_ModelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	m_ViewMatrixLocation = glGetUniformLocation(shaderProgramID, "viewMatrix");
	m_ProjectionMatrixLocation = glGetUniformLocation(shaderProgramID, "projectionMatrix");

	m_CameraPositionLocation = glGetUniformLocation(shaderProgramID, "cameraPosition");
	m_LightDirectionLocation = glGetUniformLocation(shaderProgramID, "lightDirection");

	m_AmbientLightColourLocation = glGetUniformLocation(shaderProgramID, "ambientLightColour");
	m_DiffuseLightColourLocation = glGetUniformLocation(shaderProgramID, "diffuseLightColour");
	m_SpecularLightColourLocation = glGetUniformLocation(shaderProgramID, "specularLightColour");

	m_AmbientMaterialColourLocation = glGetUniformLocation(shaderProgramID, "ambientMaterialColour");
	m_DiffuseMaterialColourLocation = glGetUniformLocation(shaderProgramID, "diffuseMaterialColour");
	m_SpecularMaterialColourLocation = glGetUniformLocation(shaderProgramID, "specularMaterialColour");

	m_SpecularPowerLocation = glGetUniformLocation(shaderProgramID, "specularPower");
}

void ShaderUniforms::update(Camera* camera, Light* light, Material* material, glm::mat4* modelMatrix)
{
	// Send shader values
	glUniform1i(m_TextureLocation, 0);

	glUniformMatrix4fv(m_ModelMatrixLocation, 1, GL_FALSE, value_ptr(*modelMatrix));
	glUniformMatrix4fv(m_ViewMatrixLocation, 1, GL_FALSE, value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(m_ProjectionMatrixLocation, 1, GL_FALSE, value_ptr(camera->getProjectionMatrix()));

	// Always send vec4s for efficiency, vec3s get ammended with a homogeneous w=0 for direction, or w=1 for position.
	glUniform4fv(m_CameraPositionLocation, 1, value_ptr(glm::vec4(camera->worldLocation->getPosition(), 1.0f)));
	glUniform4fv(m_LightDirectionLocation, 1, value_ptr(glm::vec4(light->getDirection(), 0.0f)));

	glUniform4fv(m_AmbientLightColourLocation, 1, value_ptr(light->colour->getAmbientColour()));
	glUniform4fv(m_DiffuseLightColourLocation, 1, value_ptr(light->colour->getDiffuseColour()));
	glUniform4fv(m_SpecularLightColourLocation, 1, value_ptr(light->colour->getSpecularColour()));

	glUniform4fv(m_AmbientMaterialColourLocation, 1, value_ptr(material->getAmbientColour()));
	glUniform4fv(m_DiffuseMaterialColourLocation, 1, value_ptr(material->getDiffuseColour()));
	glUniform4fv(m_SpecularMaterialColourLocation, 1, value_ptr(material->getSpecularColour()));

	glUniform1f(m_SpecularPowerLocation, material->getSpecularPower());

}
