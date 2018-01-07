#pragma once

#include <SDL.h>
#include "GameObjectLoader.h"
#include "Raycast.h"
#include "PhysicsEngine.h"

class Controls
{
public:
	Controls();
	~Controls();
	void destroy();

	void init(GameObjectLoader* gameObjects, Raycast* raycast, PhysicsEngine* dynamicsWorld);

	void update();

	bool isRunning() { return m_IsRunning; }

private:
	bool m_IsRunning;
	
	// SDL Event structure, this will be checked in the while loop
	SDL_Event m_SDLEvent;
	GameObjectLoader* m_GameObjects;
	Raycast* m_Raycast;
	PhysicsEngine* m_DynamicsWorld;
};

