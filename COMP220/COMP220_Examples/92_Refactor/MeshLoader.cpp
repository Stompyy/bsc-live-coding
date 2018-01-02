#include "MeshLoader.h"

// https://gamedev.stackexchange.com/questions/26382/i-cant-figure-out-how-to-animate-my-loaded-model-with-assimp
//
//https://www.youtube.com/watch?v=F-kcaonjHf8&index=2&list=PLRIWtICgwaX2tKWCxdeB7Wv_rTET9JtWW
int currentBoneID = 0;
std::map<std::string, int> BoneMap;

bool loadAnimationFromFile(const std::string & filename, AnimationClip ** clip)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);

	if (!scene)
	{
		printf("Model Loading Error - %s\n", importer.GetErrorString());
		return false;
	}

	if (!scene->HasAnimations())
		return false;

	//the file we are using has one animation per file
	aiAnimation * currentAnimation = scene->mAnimations[0];

	for (int i = 0; i < currentAnimation->mNumChannels; i++)
	{
		aiNodeAnim *currentNode = currentAnimation->mChannels[i];
		//printf("Animation Node %s\n", currentNode->mNodeName.C_Str());

		aiVector3D position = currentNode->mPositionKeys[0].mValue;
		aiVector3D scale = currentNode->mScalingKeys[0].mValue;
		aiQuaternion rotation = currentNode->mRotationKeys[0].mValue;
	}


	return true;
}


void processNode(aiNode * parentNode, Joint *parentJoint)
{
	Joint * pJoint = parentJoint;
	for (int i = 0; i < parentNode->mNumChildren; i++)
	{
		std::string nodeName = std::string(parentNode->mChildren[i]->mName.C_Str());
		if (BoneMap.find(nodeName) != BoneMap.end())
		{
			pJoint = new Joint(BoneMap[nodeName], nodeName, parentJoint->getBindTransform());
			parentJoint->addChildJoint(pJoint);
		}

		processNode(parentNode->mChildren[i], pJoint);
	}
}

std::vector<Mesh*> loadMeshFromFilename(const std::string& filename)
{
	std::vector<Mesh*> meshes = {};
	Assimp::Importer importer;

	//const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);
	const aiScene* scene = importer.ReadFile(filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);


	if (scene == nullptr)
	{
		printf("Error loading model %s", importer.GetErrorString());
		return meshes;
	}
	if (!scene)
	{
		printf("Model Loading Error - %s\n", importer.GetErrorString());
		return meshes;
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		const aiMesh* currentAIMesh = scene->mMeshes[m];

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

	}
	return meshes;
}


MeshLoader::MeshLoader(std::vector<std::string> meshFileNames)
{
	// Ensures empty starting value
	m_MeshMap.clear();

	for (std::string meshName : meshFileNames) 
	{
		m_MeshMap[meshName] = loadMeshFromFilename(meshName);
	}
}


MeshLoader::~MeshLoader()
{
}
