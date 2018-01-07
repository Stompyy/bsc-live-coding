#include "Controls.h"



Controls::Controls()
{
	m_IsRunning = true;

	m_GameObjects = new GameObjectLoader();
	m_Raycast = new Raycast();
	m_DynamicsWorld = new PhysicsEngine();
}


Controls::~Controls()
{
	destroy();
}

void Controls::destroy()
{
	//if (m_DynamicsWorld)	{ delete m_DynamicsWorld;	m_DynamicsWorld = nullptr; }
	//if (m_Raycast)			{ delete m_Raycast;			m_Raycast = nullptr; }
	//if (m_GameObjects)		{ delete m_GameObjects;		m_GameObjects = nullptr; }
}

void  Controls::init(GameObjectLoader* gameObjects, Raycast* raycast, PhysicsEngine* dynamicsWorld)
{
	m_GameObjects = gameObjects;
	m_Raycast = raycast;
	m_DynamicsWorld = dynamicsWorld;
}

void Controls::update()
{
	// Poll for the events which have happened in this frame
	//https://wiki.libsdl.org/SDL_PollEvent
	while (SDL_PollEvent(&m_SDLEvent))
	{
		// Switch case for every message we are interested in
		switch (m_SDLEvent.type)
		{
			// QUIT Message, usually called when the window has been closed
		case SDL_QUIT:
			m_IsRunning = false;
			break;

			// MOUSEMOTION Message, called when the mouse has been moved 
		case SDL_MOUSEMOTION:
			m_GameObjects->getPlayer()->getCamera()->turn(m_SDLEvent.motion.xrel, m_SDLEvent.motion.yrel);
			break;

			// MOUSEBUTTONDOWN message, called when a mouse button has been pressed down
		case SDL_MOUSEBUTTONDOWN:
			// Check button code of the pressed mouse button
			switch (m_SDLEvent.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				// Fire off a raycast
				m_Raycast->update(m_GameObjects->getPlayer()->getCamera(), m_DynamicsWorld->getDynamicsWorld());
				btRigidBody* hitBody = m_Raycast->getHitObject();
				if (hitBody)
				{
					GameObject* hitGameObject = (GameObject*)hitBody->getUserPointer();
					if (hitGameObject)
					{
						printf("puta: %s \n", (char*)&hitGameObject->getName());
					}
				}
				break;
			}
			}
			// SDL_MOUSEWHEEL message, called when the mouse wheel has been moved
		case SDL_MOUSEWHEEL:
		{
			// Check mouse wheel orientation
			switch (m_SDLEvent.wheel.direction)
			{
				// Standard mouse wheel orientation
			case SDL_MOUSEWHEEL_NORMAL:
			{
				// Zoom in/out on player by changing the length of the third person camera boom
				// Scroll forwards
				if (m_SDLEvent.wheel.y == 1)
					m_GameObjects->getPlayer()->getCamera()->moveCloser();

				// Scroll backwards
				else if (m_SDLEvent.wheel.y == -1)
					m_GameObjects->getPlayer()->getCamera()->moveAway();
				break;
			}
			// Mouse wheel can be set up flipped (i.e. MacOS) this detects this
			case SDL_MOUSEWHEEL_FLIPPED:
			{
				// Zoom in/out on player by changing the length of the third person camera boom
				// Scroll forwards
				if (m_SDLEvent.wheel.y == 1)
					m_GameObjects->getPlayer()->getCamera()->moveCloser();

				// Scroll backwards
				else if (m_SDLEvent.wheel.y == -1)
					m_GameObjects->getPlayer()->getCamera()->moveAway();
				break;
			}
			}
		}
		// KEYDOWN message, called when a key has been pressed down
		case SDL_KEYDOWN:
			// Check the actual key code of the key that has been pressed
			// integer arguments are the relative direction to the function name. i.e. moveForward(-1) is move backwards
			switch (m_SDLEvent.key.keysym.sym)
			{
				// Escape key
			case SDLK_ESCAPE:
				m_IsRunning = false;
				break;

			case SDLK_w:
				// Move Forward
				m_GameObjects->getPlayer()->moveForward(1);
				break;

			case SDLK_s:
				// Move backwards
				m_GameObjects->getPlayer()->moveForward(-1);
				break;

			case SDLK_a:
				// Move left
				m_GameObjects->getPlayer()->moveRight(-1);
				break;

			case SDLK_d:
				// Move right
				m_GameObjects->getPlayer()->moveRight(1);
				break;

			case SDLK_e:
				// Jump up
				m_GameObjects->getPlayer()->jumpUp();
				break;

			case SDLK_SPACE:
				// Jump forward
				m_GameObjects->getPlayer()->jumpForward();
				break;

			case SDLK_LSHIFT:
				// toggle to run
				if (m_GameObjects->getPlayer()->isRunning()) m_GameObjects->getPlayer()->walk();
				else m_GameObjects->getPlayer()->run();
				break;

			case SDLK_UP:
				// Zoom in/out on player by changing the length of the third person camera boom
				// Using to debug FBX textures! Alternative to other zoom in/out if no mouse wheel available, i.e. trackpad
				// zoom in on player
				m_GameObjects->getPlayer()->getCamera()->moveCloser();
				break;

			case SDLK_DOWN:
				// zoom out on player
				m_GameObjects->getPlayer()->getCamera()->moveAway();
				break;

			default:
				break;
			}
		case SDL_KEYUP:
		{
			// Check the actual key code of the key that has been released
			switch (m_SDLEvent.key.keysym.sym)
			{
			case SDLK_LSHIFT:
				// Toggling now, as holding messes it all up!
				//player->walk();
				break;
			}
		}
		}
	}
}
