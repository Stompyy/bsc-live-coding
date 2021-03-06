#pragma once

#include <string>
#include <vector>
#include <map>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Mesh.h"
#include "ErrorMessage.h"

using namespace std;
using namespace Assimp;

class MeshLoader
{
public:
	MeshLoader(); 
	~MeshLoader();

	void init(ErrorMessage* errorMessage, const std::vector<std::string> meshFileNames);

	void destroy();

	// Error checking here if nullptr
	std::vector<Mesh*> getMeshes(const std::string& meshName); // {  }
	std::vector<Mesh*> loadMeshFromFilename(const std::string& filename);

private: 
	Importer m_Importer;
	ErrorMessage* m_ErrorMessage;
	const aiScene* m_Scene;

	// Create an empty texture map. Aim to only load in each texture once, but able to reuse.
	std::map<std::string, std::vector<Mesh*>> m_MeshMap;

};


