#include "MeshLoader.h"

// https://gamedev.stackexchange.com/questions/26382/i-cant-figure-out-how-to-animate-my-loaded-model-with-assimp
//
//https://www.youtube.com/watch?v=F-kcaonjHf8&index=2&list=PLRIWtICgwaX2tKWCxdeB7Wv_rTET9JtWW
int currentBoneID = 0;
std::map<std::string, int> BoneMap;


GLint TextureFromFile(const char* path)//, string directory)
{
/*	//Generate texture ID and load texture data 
	string filename = string(path);
	//filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
*/
	return 0;//temp
	}

// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as an FBXTexture struct.
vector<FBXTexture> MeshLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<FBXTexture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		for (GLuint j = 0; j < m_FBXTextures.size(); j++)
		{
			if (m_FBXTextures[j].path == str)
			{
				textures.push_back(m_FBXTextures[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			FBXTexture texture;
			texture.id = TextureFromFile(str.C_Str());//, this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->m_FBXTextures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

std::vector<Mesh*> MeshLoader::loadMeshFromFilename(const std::string& filename)
{
	std::vector<Mesh*> meshes = {};
	//Assimp::Importer importer;

	//const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);
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
/*			// https://www.opengl.org/discussion_boards/showthread.php/199257-Problem-loading-3d-Model?highlight=fbx+texture+assimp
			if (currentAIMesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = scene->mMaterials[currentAIMesh->mMaterialIndex];
				// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
				// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
				// Same applies to other texture as the following list summarizes:
				// Diffuse: texture_diffuseN
				// Specular: texture_specularN
				// Normal: texture_normalN

				// 1. Diffuse maps
				vector<FBXTexture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				m_FBXTextures.insert(m_FBXTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
				// 2. Specular maps
				vector<FBXTexture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
				m_FBXTextures.insert(m_FBXTextures.end(), specularMaps.begin(), specularMaps.end());
			}
*/
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
	// Ensures empty starting value
	m_MeshMap.clear();
	m_FBXTextures.clear();
}
void MeshLoader::init(const std::vector<std::string> meshFileNames)
{
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