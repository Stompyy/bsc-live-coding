#pragma once

#include <string>
#include <vector>
#include <map>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include "shader.h"
#include "ErrorMessage.h"

struct ShaderInfo
	// Contains the vertex shader filename to load, the fragment shader filename to load, and an identifying string to use as a map key. Sensible naming convention needed here i.e. trexShader
{
	ShaderInfo(std::string shaderName, std::string vertShader, std::string fragShader) : shaderName(shaderName), vertShader(vertShader), fragShader(fragShader) {}
	std::string shaderName, vertShader, fragShader;
};

class ShaderLoader
	// Preload in all shader combinations needed so as to be able to reuse, rather than just reload everytime
{
public:
	// Initialise with all needed shaders
	ShaderLoader();
	~ShaderLoader();

	void init(ErrorMessage* errorMessage, const std::vector<ShaderInfo*>& shaderFileNames);
	void destroy();

	// Retrieves the shaderID from the map - NEEDS ERROR CHECKING!
	GLuint getShaderID(const std::string& shaderName);

private:
	ErrorMessage* m_ErrorMessage;

	// Create an empty shader map.
	std::map<std::string, GLuint> m_ShaderMap;
};

