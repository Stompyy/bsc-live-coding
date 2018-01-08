#include "ShaderLoader.h"

ShaderLoader::ShaderLoader()
{
	// Ensures empty starting value
	m_ShaderMap.clear();

	m_ErrorMessage = new ErrorMessage();
}

void ShaderLoader::init(ErrorMessage* errorMessage, const std::vector<ShaderInfo*>& shaderFileNames)
{
	m_ErrorMessage = errorMessage;

	// Builds the map of GLuint shaderIDs and their identifying string keys
	for (ShaderInfo* shaderInfo : shaderFileNames)
	{
		m_ShaderMap[shaderInfo->shaderName] = LoadShaders(shaderInfo->vertShader.c_str(), shaderInfo->fragShader.c_str());
	}
}

ShaderLoader::~ShaderLoader()
{
	destroy();
}

void ShaderLoader::destroy()
{
}

GLuint ShaderLoader::getShaderID(const std::string & shaderName)
{
	if (m_ShaderMap[shaderName] <= 0)
	{
		m_ErrorMessage->showErrorMessage("shader not found, check correct map key is being used.");
	}
	return m_ShaderMap[shaderName];
}
