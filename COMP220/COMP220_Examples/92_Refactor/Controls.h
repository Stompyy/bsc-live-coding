#pragma once

#include <SDL.h>
#include "GameObjectLoader.h"
#include "Raycast.h"
#include "PhysicsEngine.h"
#include "GameManager.h"

class Controls
{
public:
	Controls();
	~Controls();
	void destroy();

	void init(GameManager* gameManager, GameObjectLoader* gameObjects, Raycast* raycast, PhysicsEngine* dynamicsWorld);

	void update();

private:
	GameManager* m_GameManager;
	SDL_Event m_SDLEvent;
	GameObjectLoader* m_GameObjects;
	Raycast* m_Raycast;
	PhysicsEngine* m_DynamicsWorld;
};

