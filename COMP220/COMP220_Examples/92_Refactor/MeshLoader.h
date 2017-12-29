#pragma once

#include <string>
#include <vector>
#include <map>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"

class MeshLoader
{
public:
	MeshLoader(std::vector<std::string> meshFileNames);
	~MeshLoader();

	// Error checking here if nullptr
	std::vector<Mesh*> getMeshes(const std::string& meshName) { return m_MeshMap[meshName]; }

private:
	// Create an empty texture map. Aim to only load in each texture once, but able to reuse.
	std::map<std::string, std::vector<Mesh*>> m_MeshMap;
};

