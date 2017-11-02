#include "model.h"

bool loadModelFromFile(const std::string & filename, GLuint VBO, GLuint EBO, unsigned int & numberOfVerts, unsigned int & numberOfIndices)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);

	if (scene == nullptr)
	{
		printf("Error loading model %s", importer.GetErrorString());
		return false;
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		const aiMesh* currentAIMesh = scene->mMeshes[m];
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

			vertices.push_back(myVertex);
		}
		for (unsigned int f = 0; f < currentAIMesh->mNumFaces; f++)
		{
			const aiFace currentFace = currentAIMesh->mFaces[f];
			indices.push_back(currentFace.mIndices[0]);
			indices.push_back(currentFace.mIndices[1]);
			indices.push_back(currentFace.mIndices[2]);
		}
	}

	numberOfVerts = vertices.size();
	numberOfIndices = indices.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numberOfVerts * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	return true;
}

bool loadMeshFromFile(const std::string& filename, std::vector<Mesh*>& meshes)
{
	Assimp::Importer importer;

	//const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);
	const aiScene* scene = importer.ReadFile(filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);


	if (scene == nullptr)
	{
		printf("Error loading model %s", importer.GetErrorString());
		return false;
	}
	if (!scene)
	{
		printf("Model Loading Error - %s\n", importer.GetErrorString());
		return false;
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
	return true;
}
