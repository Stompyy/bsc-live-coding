#include "GameObjectLoader.h"



GameObjectLoader::GameObjectLoader()
{
	// Ensures empty starting value
	m_GameObjectMap.clear();
	m_GameObjectList.clear();

	m_ErrorMessage = new ErrorMessage();
}

GameObjectLoader::~GameObjectLoader()
{
	destroy();
}

void GameObjectLoader::destroy()
{
	if (m_ErrorMessage) { delete m_ErrorMessage; m_ErrorMessage = nullptr; }

	auto iter = m_GameObjectList.begin();
	while (iter != m_GameObjectList.end())
	{
		if ((*iter))
		{
			(*iter)->destroy();
			delete (*iter);
			iter = m_GameObjectList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

// Uses the GameObject class's own init() function
void GameObjectLoader::init(MeshLoader* meshLoader, TextureLoader* textureLoader, ShaderLoader* shaderLoader, PhysicsEngine* dynamicsWorld, const std::vector<GameObjectInfo*>& gameObjectInfoList)
{
	for (GameObjectInfo* constructionInfo : gameObjectInfoList)
	{
		GameObject* newGameObject = new GameObject();
		newGameObject->init(
			constructionInfo->gameObjectName,
			meshLoader->getMeshes(constructionInfo->meshFileName),
			textureLoader->getTextureID(constructionInfo->textureFileName),
			shaderLoader->getShaderID(constructionInfo->shaderName),
			constructionInfo->initialTransform,
			constructionInfo->mass,
			constructionInfo->collisionSize
		);

		// Add to the map with the gameObjectName as the map key
		m_GameObjectMap[constructionInfo->gameObjectName] = newGameObject;

		// Add the newly created Rigid/body to the physics simulation
		dynamicsWorld->addRigidBody(newGameObject->getPhysics()->getRigidBody());

		// Add to the vector of gameObject references to be used in destroy()
		m_GameObjectList.push_back(newGameObject);
	}
}

// Similar to init() function, but adds a Player Object instead of a standard GameObject
void GameObjectLoader::addPlayer(MeshLoader* meshLoader, TextureLoader* textureLoader, ShaderLoader* shaderLoader, PhysicsEngine* dynamicsWorld, GameObjectInfo* constructionInfo)
{
	Player* player = new Player();
	player->init(
		constructionInfo->gameObjectName,
		meshLoader->getMeshes(constructionInfo->meshFileName),
		textureLoader->getTextureID(constructionInfo->textureFileName),
		shaderLoader->getShaderID(constructionInfo->shaderName),
		constructionInfo->initialTransform,
		constructionInfo->mass,
		constructionInfo->collisionSize
	);

	// Add to the map with the gameObjectName as the map key
	m_GameObjectMap[constructionInfo->gameObjectName] = player;

	// Add the newly created Rigid/body to the physics simulation
	dynamicsWorld->addRigidBody(player->getPhysics()->getRigidBody());

	// Add to the vector of gameObject references to be used in destroy()
	m_GameObjectList.push_back(player);
}

GameObject * GameObjectLoader::getGameObject(const std::string & gameObjectName)
{
	if (m_GameObjectMap[gameObjectName] == nullptr)
	{
		m_ErrorMessage->showErrorMessage("Game Object not found, check correct map key is being used.");
	}
	return m_GameObjectMap[gameObjectName];
}

Player * GameObjectLoader::getPlayer()
{
	if (m_GameObjectMap["player"] == nullptr)
	{
		m_ErrorMessage->showErrorMessage("player not found, check correct map key is being used.");
	}
	return (Player*)m_GameObjectMap["player"];
}

Player * GameObjectLoader::getPlayer(std::string * playerGameObjectName)
{
	if (m_GameObjectMap[*playerGameObjectName] == nullptr)
	{
		m_ErrorMessage->showErrorMessage("Player not found, check correct map key is being used.");
	}
	return (Player*)m_GameObjectMap[*playerGameObjectName];
}

