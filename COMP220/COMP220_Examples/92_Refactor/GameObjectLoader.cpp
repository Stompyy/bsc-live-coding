#include "GameObjectLoader.h"



GameObjectLoader::GameObjectLoader()
{
	// Ensures empty starting value
	m_GameObjectMap.clear();
}


GameObjectLoader::~GameObjectLoader()
{
	destroy();
}

void GameObjectLoader::destroy()
{
}

// Uses the GameObject class's own init() function
void GameObjectLoader::init(MeshLoader* meshLoader, TextureLoader* textureLoader, ShaderLoader* shaderLoader, PhysicsEngine* dynamicsWorld, const std::vector<GameObjectInfo*>& gameObjectInfoList)
{
	for (GameObjectInfo* constructionInfo : gameObjectInfoList)
	{
		GameObject* newGameObject = new GameObject();
		newGameObject->init(
			meshLoader->getMeshes(constructionInfo->meshFileName),
			textureLoader->getTextureID(constructionInfo->textureFileName),
			shaderLoader->getShaderID(constructionInfo->shaderName),
			constructionInfo->initialPosition,
			constructionInfo->mass,
			constructionInfo->collisionSize
		);

		// Add to the map with the gameObjectName as the map key
		m_GameObjectMap[constructionInfo->gameObjectName] = newGameObject;

		// Add the newly created Rigid/body to the physics simulation
		dynamicsWorld->addRigidBody(newGameObject->getPhysics()->getRigidBody());
	}
}

// Similar to init() function, but adds a Player Object instead of a standard GameObject
void GameObjectLoader::addPlayer(MeshLoader* meshLoader, TextureLoader* textureLoader, ShaderLoader* shaderLoader, PhysicsEngine* dynamicsWorld, GameObjectInfo* constructionInfo)
{
	Player* player = new Player();
	player->init(
		meshLoader->getMeshes(constructionInfo->meshFileName),
		textureLoader->getTextureID(constructionInfo->textureFileName),
		shaderLoader->getShaderID(constructionInfo->shaderName),
		constructionInfo->initialPosition,
		constructionInfo->mass,
		constructionInfo->collisionSize
	);

	// Add to the map with the gameObjectName as the map key
	m_GameObjectMap[constructionInfo->gameObjectName] = player;

	// Add the newly created Rigid/body to the physics simulation
	dynamicsWorld->addRigidBody(player->getPhysics()->getRigidBody());
}
