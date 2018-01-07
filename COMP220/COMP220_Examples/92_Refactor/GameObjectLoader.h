#pragma once

#include <vector>
#include <map>

#include "GameObject.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "PhysicsEngine.h"
#include "Player.h"
#include "ErrorMessage.h"

struct GameObjectInfo
{
	GameObjectInfo(
		std::string gameObjectName,
		std::string meshFileName,
		std::string textureFileName,
		std::string shaderName,
		Transform* initialTransform,
		float mass,
		btVector3& collisionSize)
		:
		gameObjectName(gameObjectName),
		meshFileName(meshFileName),
		textureFileName(textureFileName),
		shaderName(shaderName),
		initialTransform(initialTransform),
		mass(mass),
		collisionSize(collisionSize) {}
	
	std::string gameObjectName, meshFileName, textureFileName, shaderName;
	Transform* initialTransform;
	float mass;
	btVector3 collisionSize;
};

class GameObjectLoader
{
public:
	GameObjectLoader();
	~GameObjectLoader();

	void destroy();

	// Fill out the map with gameObjects. Takes a vector of GameObjectInfo*s, and initialises each one before adding to map with gameObjectName field as the map key
	void init(
		MeshLoader* meshLoader, 
		TextureLoader* textureLoader,
		ShaderLoader* shaderLoader,
		PhysicsEngine* dynamicsWorld,
		const std::vector<GameObjectInfo*>& gameObjectInfoList
	);

	// Add the player object to the gameObject map. Similar to init() function, but adds a Player Object instead of a standard GameObject
	void addPlayer(
		MeshLoader* meshLoader,
		TextureLoader* textureLoader,
		ShaderLoader* shaderLoader,
		PhysicsEngine* dynamicsWorld,
		GameObjectInfo* constructionInfo
	);

	// Get gameObject from gameObjectLoader by name
	GameObject* getGameObject(const std::string& gameObjectName);

	// Return Player object with map key "player"
	Player* getPlayer();

	// Return Player object with explicit map key name
	Player* getPlayer(std::string* playerGameObjectName);

	// Return gameObjectMap
	std::map<std::string, GameObject*> getGameObjectMap() { return m_GameObjectMap; }

	// Return gameObjectList
	std::vector<GameObject*> getGameObjectList() { return m_GameObjectList; }

private:
	// Map of GameObjects to store all GameObjects within the scene
	std::map<std::string, GameObject*> m_GameObjectMap;

	// Used to store references to the gameObjects to make it easy to delete them all in destroy()
	std::vector<GameObject*> m_GameObjectList;

	// Error message if needed
	ErrorMessage* m_ErrorMessage;
};

