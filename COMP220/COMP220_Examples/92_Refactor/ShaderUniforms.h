#pragma once

#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>

#include "Camera.h"
#include "Light.h"

class ShaderUniforms
{
public:
	ShaderUniforms();
	~ShaderUniforms();

	void init(GLuint shaderProgramID);
	void update(Camera* camera, Light* light, Material* material, glm::mat4* modelMatrix);

private:
	GLint m_TextureLocation;

	GLint m_ModelMatrixLocation;
	GLint m_ViewMatrixLocation;
	GLint m_ProjectionMatrixLocation;

	GLint m_CameraPositionLocation;
	GLint m_LightDirectionLocation;

	GLint m_AmbientLightColourLocation;
	GLint m_DiffuseLightColourLocation;
	GLint m_SpecularLightColourLocation;

	GLint m_AmbientMaterialColourLocation;
	GLint m_DiffuseMaterialColourLocation;
	GLint m_SpecularMaterialColourLocation;

	GLint m_SpecularPowerLocation;
};

