#pragma once

#include <vector>
#include <map>

#include "GameObject.h"
#include "MeshLoader.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "PhysicsEngine.h"
#include "Player.h"

struct GameObjectInfo
{
	GameObjectInfo(
		std::string gameObjectName,
		std::string meshFileName,
		std::string textureFileName,
		std::string shaderName,
		glm::vec3 initialPosition,
		float mass,
		btVector3 collisionSize)
		:
		gameObjectName(gameObjectName),
		meshFileName(meshFileName),
		textureFileName(textureFileName),
		shaderName(shaderName),
		initialPosition(initialPosition),
		mass(mass),
		collisionSize(collisionSize) {}
	
	std::string gameObjectName, meshFileName, textureFileName, shaderName;
	glm::vec3 initialPosition;
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
	GameObject* getGameObject(const std::string& gameObjectName) { return m_GameObjectMap[gameObjectName]; }

	// Return Player object with map key "player"
	Player* getPlayer() { return (Player*)m_GameObjectMap["player"]; }

	// Return Player object with explicit map key name
	Player* getPlayer(std::string* playerGameObjectName) { return (Player*)m_GameObjectMap[*playerGameObjectName]; }

	// Return gameObjectMap
	std::map<std::string, GameObject*> getGameObjectMap() { return m_GameObjectMap; }

private:
	// Map of GameObjects to store all GameObjects within the scene
	std::map<std::string, GameObject*> m_GameObjectMap;
};

