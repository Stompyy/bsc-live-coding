#include "LoadGameObjects.h"

std::vector<GameObject*>& loadGameObjects(TextureLoader* textureLoader, MeshLoader* meshLoader)
{
	std::vector<GameObject*> gameObjectList;
	gameObjectList.clear();

	GameObject* ground = new GameObject();
	ground->init(
		meshLoader->getMeshes("floor.FBX"),			// meshes
		textureLoader->getTextureID("grass.png"),	// texture
		"textureVert.glsl", "textureFrag.glsl",		// shaders
		vec3(0.0f, -5.0f, 0.0f),					// initial position
		0.0f,										// mass
		btVector3(50.0f, 1.0f, 50.0f)				// collision size
	);
	gameObjectList.push_back(ground);

	GameObject* trex = new GameObject();
	trex->init(
		meshLoader->getMeshes("Trex.FBX"),
		textureLoader->getTextureID("TrexColour.jpg"),
		"lightingVert.glsl", "lightingFrag.glsl",
		vec3(0.0f, 200.0f, 0.0f),
		1.0f,
		btVector3(0.0f, 0.0f, 0.0f)
	);
	gameObjectList.push_back(trex);

	GameObject* tank3 = new GameObject();
	tank3->init(
		meshLoader->getMeshes("tank.FBX"),
		textureLoader->getTextureID("tankColour.png"),
		"lightingVert.glsl", "lightingFragTank.glsl",
		vec3(-15.0f, 0.0f, 15.0f),
		1.0f,
		btVector3(0.0f, 0.0f, 0.0f)
	);
	gameObjectList.push_back(tank3);

	GameObject* tank4 = new GameObject();
	tank4->init(
		meshLoader->getMeshes("tank.FBX"),
		textureLoader->getTextureID("tankColour.png"),
		"lightingVert.glsl", "lightingFragTank.glsl",
		vec3(15.0f, 0.0f, -15.0f),
		1.0f,
		btVector3(0.0f, 0.0f, 0.0f)
	);
	gameObjectList.push_back(tank4);

	GameObject* tank = new GameObject();
	tank->init(
		meshLoader->getMeshes("tank.FBX"),
		textureLoader->getTextureID("tankColour.png"),
		"lightingVert.glsl", "lightingFragTank.glsl",
		vec3(15.0f, 0.0f, 15.0f),
		1.0f,
		btVector3(0.0f, 0.0f, 0.0f)
	);
	gameObjectList.push_back(tank);

	GameObject* tank2 = new GameObject();
	tank2->init(
		meshLoader->getMeshes("tank.FBX"),
		textureLoader->getTextureID("tankColour.png"),
		"lightingVert.glsl", "lightingFragTank.glsl",
		vec3(-15.0f, 0.0f, -15.0f),
		1.0f,
		btVector3(0.0f, 0.0f, 0.0f)
	);
	gameObjectList.push_back(tank2);

	GameObject* player = new GameObject();
	player->init(
		meshLoader->getMeshes("archer.FBX"),
		textureLoader->getTextureID("TrexColour.jpg"),
		"lightingVert.glsl", "lightingFrag.glsl",
		vec3(10.0f, 0.0f, 10.0f),
		1.0f,
		btVector3(0.0f, 0.0f, 0.0f)
	);
	player->transform->setScale(0.02f);
	gameObjectList.push_back(player);

	return gameObjectList;
}
