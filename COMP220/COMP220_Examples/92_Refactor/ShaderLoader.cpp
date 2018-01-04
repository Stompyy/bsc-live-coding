#include "ShaderLoader.h"

ShaderLoader::ShaderLoader(std::vector<ShaderInfo*>& shaderFileNames)
{
	// Ensures empty starting value
	m_ShaderMap.clear();

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
/*	if (m_ShaderMap.size() > 0)
	{
		auto iter = m_ShaderMap.begin();
		while (iter != m_ShaderMap.end())
		{
			if ((*iter))
			{
				(*iter)->destroy();
				delete (*iter);
				iter = m_ShaderMap.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
*/
}
