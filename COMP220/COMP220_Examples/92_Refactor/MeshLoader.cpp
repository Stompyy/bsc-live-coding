#include "MeshLoader.h"

std::vector<Mesh*> MeshLoader::getMeshes(const std::string & meshName)
{
	if (m_MeshMap[meshName].size() == 0)
	{
		m_ErrorMessage->showErrorMessage("Meshes not found, check correct map key is being used.");
	}
	return m_MeshMap[meshName];
}

std::vector<Mesh*> MeshLoader::loadMeshFromFilename(const std::string& filename)
{
	std::vector<Mesh*> meshes = {};
	m_Scene = m_Importer.ReadFile(filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);


	if (m_Scene == nullptr)
	{
		printf("Error loading model %s", m_Importer.GetErrorString());
		return meshes;
	}
	if (!m_Scene)
	{
		printf("Model Loading Error - %s\n", m_Importer.GetErrorString());
		return meshes;
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<FBXTexture> textures;

	for (unsigned int m = 0; m < m_Scene->mNumMeshes; m++)
	{
		const aiMesh* currentAIMesh = m_Scene->mMeshes[m];

		// For each mesh loaded in scene, copy all information into our Mesh class version, before at the end, pushing onto the meshes vector<Mesh>
		Mesh* myCurrentMesh = new Mesh();
		myCurrentMesh->init();

		for (unsigned int v = 0; v < currentAIMesh->mNumVertices; v++)
		{
			const aiVector3D currentAIPosition = currentAIMesh->mVertices[v];

			Vertex myVertex;
			myVertex.x = currentAIPosition.x;
			myVertex.y = currentAIPosition.y;
			myVertex.z = currentAIPosition.z;
			myVertex.r = 1.0f; myVertex.g = 1.0f; myVertex.b = 1.0f; myVertex.a = 1.0f;
			myVertex.tu = 0.0f; myVertex.tv = 0.0f;

			if (currentAIMesh->HasTextureCoords(0))
			{
				const aiVector3D currentTextureCoords = currentAIMesh->mTextureCoords[0][v];
				myVertex.tu = currentTextureCoords.x;
				myVertex.tv = currentTextureCoords.y;
			}
			if (currentAIMesh->HasVertexColors(0))
			{
				const aiColor4D currentColour = currentAIMesh->mColors[0][v];
				myVertex.r = currentColour.r;
				myVertex.g = currentColour.g;
				myVertex.b = currentColour.b;
				myVertex.a = currentColour.a;
			}
			if (currentAIMesh->HasNormals())
			{
				const aiVector3D currentNormals = currentAIMesh->mNormals[v];
				myVertex.normalX = currentNormals.x;
				myVertex.normalY = currentNormals.y;
				myVertex.normalZ = currentNormals.z;
			}

			vertices.push_back(myVertex);
		}
		for (unsigned int f = 0; f < currentAIMesh->mNumFaces; f++)
		{
			const aiFace currentFace = currentAIMesh->mFaces[f];
			indices.push_back(currentFace.mIndices[0]);
			indices.push_back(currentFace.mIndices[1]);
			indices.push_back(currentFace.mIndices[2]);
		}

		myCurrentMesh->CopyBufferData(vertices.data(), vertices.size(), indices.data(), indices.size());
		meshes.push_back(myCurrentMesh);

		vertices.clear();
		indices.clear();
		textures.clear();

	}
	return meshes;
}

MeshLoader::MeshLoader()
{
	m_ErrorMessage = new ErrorMessage();

	// Ensures empty starting value
	m_MeshMap.clear();
}
void MeshLoader::init(ErrorMessage* errorMessage, const std::vector<std::string> meshFileNames)
{
	m_ErrorMessage = errorMessage;

	// Builds the map of meshes and their identifying string keys
	for (std::string meshName : meshFileNames)
	{
		m_MeshMap[meshName] = loadMeshFromFilename(meshName);
	}
}

void MeshLoader::destroy()
{
}

MeshLoader::~MeshLoader()
{
	destroy();
}