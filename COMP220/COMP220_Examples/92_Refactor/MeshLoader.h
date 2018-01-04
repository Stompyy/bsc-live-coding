#pragma once

#include <string>
#include <vector>
#include <map>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/DefaultLogger.hpp>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
//#include <SOIL.h>

#include "Mesh.h"
#include "Animation.h"
#include "Joint.h"

using namespace std;
using namespace Assimp;

class MeshLoader
{
public:
	MeshLoader(std::vector<std::string> meshFileNames);
	~MeshLoader();

	// Error checking here if nullptr
	std::vector<Mesh*> getMeshes(const std::string& meshName) { return m_MeshMap[meshName]; }
	std::vector<Mesh*> loadMeshFromFilename(const std::string& filename);
	vector<FBXTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

private:
	Importer m_Importer;
	const aiScene* m_Scene;

	// Create an empty texture map. Aim to only load in each texture once, but able to reuse.
	std::map<std::string, std::vector<Mesh*>> m_MeshMap;

	std::vector<FBXTexture> m_FBXTextures;

};


