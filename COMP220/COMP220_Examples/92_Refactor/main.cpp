
//main.cpp - defines the entry point of the application

#include "main.h"

/*
vec3 btQuatToGlmVec3(const btQuaternion& q)
// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
{
	double roll, pitch, yaw;

	// roll (x-axis rotation)
	double sinr = +2.0 * (q.getW() * q.getX() + q.getY() * q.getZ());
	double cosr = +1.0 - 2.0 * (q.getX() * q.getX() + q.getY() * q.getY());
	roll = atan2(sinr, cosr);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (q.getW() * q.getY() - q.getZ() * q.getX());
	if (fabs(sinp) >= 1)
		pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		pitch = asin(sinp);

	// yaw (z-axis rotation)
	double siny = +2.0 * (q.getW() * q.getZ() + q.getX() * q.getY());
	double cosy = +1.0 - 2.0 * (q.getY() * q.getY() + q.getZ() * q.getZ());
	yaw = atan2(siny, cosy); 

	return vec3(roll, pitch, yaw);
}
*/

// Initialise SDL
SDL_Window* SDL_Init()
{
	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Display an error message box
		//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_Init failed", NULL);
		return nullptr;
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	SDL_Window* SDL_window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (SDL_window == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_CreateWindow failed", NULL);
		//Close the SDL Library
		//https://wiki.libsdl.org/SDL_Quit
		SDL_Quit();
		return nullptr;
	}
	return SDL_window;
}

// Get the GL_Context
SDL_GLContext GL_Init(SDL_Window* SDL_window)
{
	// 3.2 core profile version of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create the GL context
	SDL_GLContext GL_Context = SDL_GL_CreateContext(SDL_window);
	if (GL_Context == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL GL Create Context failed", NULL);
		SDL_DestroyWindow(SDL_window);
		SDL_Quit();
		return nullptr;
	}
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, (char*)glewGetErrorString(glewError), "GLEW Init Failed", NULL);
		return nullptr;
	}
	return GL_Context;
}

// Destroy SDL window, delete GL_Context, and quit
void close(SDL_Window* SDL_window, SDL_GLContext GL_Context)
{
	SDL_GL_DeleteContext(GL_Context);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(SDL_window);

	IMG_Quit();
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();
}

void update(GameObjectLoader* gameObjects, PostProcessing* postProcessing, PhysicsEngine* dynamicsWorld)
{
	// Update the player's physics position to the new position set by the controls
	gameObjects->getPlayer()->updateMovement();

	// Bind the frame buffer
	postProcessing->bindFrameBuffer();

	// Advance the physics simulation
	dynamicsWorld->getDynamicsWorld()->stepSimulation(1.0f / 60.0f, 10);

	// Apply physics simulation to every GameObject
	for (auto const& mapElement : gameObjects->getGameObjectMap())
	{
		// Retrieve the GameObject* from the mapElement <std::string, GameObject*>
		GameObject* gameObject = mapElement.second;

		// Update the model matrix (TRS!)
		gameObject->update();
	};
}

void render(GameObjectLoader* gameObjects, Light* lightOne, Light* lightTwo)
{
	for (auto const& mapElement : gameObjects->getGameObjectMap())
	{
		// Retrieve the GameObject* from the mapElement <std::string, GameObject*>
		GameObject* gameObject = mapElement.second;

		// Pass all values to the shader
		gameObject->preRender(gameObjects->getPlayer()->getCamera(), lightOne, lightTwo);

		// Draw
		gameObject->render();
	}
}

/**		_____		_______		 ___    ___		   ______		 _____		 _____		   ______		
*	  / _____\	   / _____ \	|   \  |   \	  |  __  \		|_____ \	|_____ \	  / ____ \	
*	 / /		  /	/	  \ \	| |\ \ | |\ \	  | |__/ |			  \ \		  \ \	 / /	\ \	
*	| |		 __	 | |	   | |	| | \ \| | \ \	  |  ___/		     / /	     / /	| |		 | |	
*	 \ \____/ /	  \ \_____/ /	| |  \___|	\ \	  | |			 __/ /___	 __/ /___	 \ \____/ /	
*	  \______/	   \_______/	|_|			 \_\  |_|			|________|	|________|	  \______/	
*
*	https://github.com/Stompyy/bsc-live-coding
*/

int main(int argc, char* args[])
{
	// Initialise both SDL and GL
	SDL_Window* SDL_window = SDL_Init();
	SDL_GLContext GL_Context = GL_Init(SDL_window);

	// If either did not initialise then exit
	if ((SDL_window == nullptr) | (GL_Context == nullptr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "Initialisation failed", NULL);
		close(SDL_window, GL_Context);
	}

	// Create a physics simulation using bullet
	PhysicsEngine* dynamicsWorld = new PhysicsEngine();

	// Load in all needed textures
	TextureLoader* textureLoader = new TextureLoader();
	textureLoader->init(std::vector<std::string>{ "TrexColour.jpg", "tankColour.png", "grass.png", "archerTex1.png" });

	// Load in all needed meshes
	MeshLoader* meshLoader = new MeshLoader();
	meshLoader->init(std::vector<std::string>{ "Trex.FBX", "tank.FBX", "floor.FBX", "archer.FBX" });

	// Load in and label all needed shaders
	ShaderLoader* shaderLoader = new ShaderLoader();
	shaderLoader->init(std::vector<ShaderInfo*>{
			new ShaderInfo("grassShader", "textureVert.glsl", "textureFrag.glsl"),
			new ShaderInfo("trexShader", "lightingVert.glsl", "lightingFrag.glsl"),
			new ShaderInfo("tankShader", "lightingVert.glsl", "lightingFragTank.glsl"),
			new ShaderInfo("postProcessingShader", "passThroughVert.glsl", "postCellNotCell.glsl")
	});

	// Post processing class initialisation
	PostProcessing* postProcessing = new PostProcessing();
	postProcessing->setPostProcessingProgramID(shaderLoader->getShaderID("postProcessingShader"));
	postProcessing->setTexture0Location(glGetUniformLocation(postProcessing->getPostProcessingProgramID(), "texture0"));

	// Single raycast instance to be reused
	Raycast* raycast = new Raycast();

	// Load in all game objects
	GameObjectLoader* gameObjects = new GameObjectLoader();
	gameObjects->init(meshLoader, textureLoader, shaderLoader, dynamicsWorld,
		std::vector<GameObjectInfo*>{
			new GameObjectInfo("ground", "floor.FBX", "grass.png", "grassShader", vec3(0.0f, -5.0f, 0.0f), 0.0f, btVector3(50.0f, 1.0f, 50.0f)),
			new GameObjectInfo("trex", "Trex.FBX", "TrexColour.jpg", "trexShader", vec3(0.0f, 0.0f, 0.0f), 1.0f, btVector3(0.0f, 0.0f, 0.0f)),
			new GameObjectInfo("tank1", "tank.FBX", "tankColour.png", "tankShader", vec3(15.0f, 0.0f, 15.0f), 1.0f, btVector3(0.0f, 0.0f, 0.0f)),
			new GameObjectInfo("tank2", "tank.FBX", "tankColour.png", "tankShader", vec3(15.0f, 0.0f, -15.0f), 1.0f, btVector3(0.0f, 0.0f, 0.0f)),
			new GameObjectInfo("tank3", "tank.FBX", "tankColour.png", "tankShader", vec3(-15.0f, 0.0f, 15.0f), 1.0f, btVector3(0.0f, 0.0f, 0.0f)),
			new GameObjectInfo("tank4", "tank.FBX", "tankColour.png", "tankShader", vec3(-15.0f, 0.0f, -15.0f), 1.0f, btVector3(0.0f, 0.0f, 0.0f)),
	});
	// Add the player in seperately as it will be a GameObject child Player class instance.
	gameObjects->addPlayer(meshLoader, textureLoader, shaderLoader, dynamicsWorld,
		new GameObjectInfo("player", "archer.FBX", "archerTex1.png", "tankShader", vec3(10.0f, 0.0f, 0.0f), 0.3f, btVector3(0.0f, 0.0f, 0.0f))
	);

	// Set up camera
	gameObjects->getPlayer()->getCamera()->setProjectionMatrix(90.0f, (1000 / 800), 0.1f, 1000.0f);

	// My player's FBX file is a very large size! Could have added a transform argument into the GameObjectLoader init(), but this will get messy and most models transforms will be  
	gameObjects->getPlayer()->getTransform()->setScale(0.015f);

	// Lights initialisation
	// Specular green!
	Light* lightOne = new Light();
	lightOne->setDirection(0.2f, -1.0f, 0.2f);
	lightOne->getColour()->setSpecularColour(0.0f, 1.0f, 0.0f);

	// Specular purple!
	Light* lightTwo = new Light();
	lightTwo->setDirection(-0.6f, -1.0f, -0.1f);
	lightTwo->getColour()->setSpecularColour(1.0f, 0.0f, 1.0f);

	// Hides the mouse and takes relative position to avoid an initial snap
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_bool(SDL_ENABLE));

	// Rendering depth test
	glEnable(GL_DEPTH_TEST);

	// Event loop, we will loop until running is set to false, if escape has been pressed or window is closed
	bool running = true;

	// SDL Event structure, this will be checked in the while loop
	SDL_Event SDLEvent;

	while (running)
	{
		// Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&SDLEvent))
		{
			// Switch case for every message we are interested in
			switch (SDLEvent.type)
			{
				// QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;

				// MOUSEMOTION Message, called when the mouse has been moved 
			case SDL_MOUSEMOTION:
				gameObjects->getPlayer()->getCamera()->turn(SDLEvent.motion.xrel, SDLEvent.motion.yrel);
				break;

				// MOUSEBUTTONDOWN message, called when a mouse button has been pressed down
			case SDL_MOUSEBUTTONDOWN:
				// Check button code of the pressed mouse button
				switch (SDLEvent.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					// Fire off a raycast
					raycast->update(gameObjects->getPlayer()->getCamera(), dynamicsWorld->getDynamicsWorld());
					btRigidBody * hitBody = raycast->getHitObject();
					if (hitBody)
					{
						GameObject *pHitGameObject = (GameObject*)hitBody->getUserPointer();
						if (pHitGameObject)
						{

						}
					}
					break;
				}
				}
				// SDL_MOUSEWHEEL message, called when the mouse wheel has been moved
			case SDL_MOUSEWHEEL:
			{
				// Check mouse wheel orientation
				switch (SDLEvent.wheel.direction)
				{
					// Standard mouse wheel orientation
				case SDL_MOUSEWHEEL_NORMAL:
				{
					// Zoom in/out on player by changing the length of the third person camera boom
					// Scroll forwards
					if (SDLEvent.wheel.y == 1)
						gameObjects->getPlayer()->getCamera()->moveCloser();

					// Scroll backwards
					else if (SDLEvent.wheel.y == -1)
						gameObjects->getPlayer()->getCamera()->moveAway();
					break;
				}
				// Mouse wheel can be set up flipped (i.e. MacOS) this detects this
				case SDL_MOUSEWHEEL_FLIPPED:
				{
					// Zoom in/out on player by changing the length of the third person camera boom
					// Scroll forwards
					if (SDLEvent.wheel.y == 1)
						gameObjects->getPlayer()->getCamera()->moveCloser();

					// Scroll backwards
					else if (SDLEvent.wheel.y == -1)
						gameObjects->getPlayer()->getCamera()->moveAway();
					break;
				}
				}
				
			}
				// KEYDOWN message, called when a key has been pressed down
			case SDL_KEYDOWN:
				// Check the actual key code of the key that has been pressed
				switch (SDLEvent.key.keysym.sym)
				{
					// Escape key
				case SDLK_ESCAPE:
					running = false;
					break;

				case SDLK_w:
					// Move Forward
					gameObjects->getPlayer()->moveForward(1);
					break;

				case SDLK_s:
					// Move backwards
					gameObjects->getPlayer()->moveForward(-1);
					break;

				case SDLK_a:
					// Move left
					gameObjects->getPlayer()->moveRight(-1);
					break;

				case SDLK_d:
					// Move right
					gameObjects->getPlayer()->moveRight(1);
					break;

				case SDLK_SPACE:
					// Jump
					gameObjects->getPlayer()->jump();
					break; 

				case SDLK_LSHIFT:
					// toggle to run
					if (gameObjects->getPlayer()->isRunning()) gameObjects->getPlayer()->walk();
					else gameObjects->getPlayer()->run();
					break;

				case SDLK_UP:
					// Zoom in/out on player by changing the length of the third person camera boom
					// Using to debug FBX textures! Alternative to other zoom in/out if no mouse wheel available, i.e. trackpad
					// zoom in on player
					gameObjects->getPlayer()->getCamera()->moveCloser();
					break;

				case SDLK_DOWN:
					// zoom out on player
					gameObjects->getPlayer()->getCamera()->moveAway();
					break;

				default:
					break;
				}
			case SDL_KEYUP:
			{
				// Check the actual key code of the key that has been released
				switch (SDLEvent.key.keysym.sym)
				{
				case SDLK_LSHIFT:
					// Toggling now, as holding messes it all up!
					//player->walk();
					break;
				}
			}
			}
		}

		// Input, logic with input, physics, graphics.
		//http://sdl.beuc.net/sdl.wiki/SDL_GetKeyState

		// Update the player, each gameObjects physics simulation, and bind the frame buffer
		update(gameObjects, postProcessing, dynamicsWorld);
		
		// Send all gameObjects values to the shader and draw
		render(gameObjects, lightOne, lightTwo);
		
		// Update the camera MVP
		gameObjects->getPlayer()->getCamera()->update();

		// Post processing frame buffer render
		postProcessing->render();

		// Swap the SDL window to display the new image
		SDL_GL_SwapWindow(SDL_window);
	}

	// Delete all Objects in reverse instantiation order
	/*
	// Important! remember all this!
	auto iter = gameObjectList.begin();
	while (iter != gameObjectList.end())
	{
	if ((*iter))
	{
	(*iter)->destroy();
	delete (*iter);
	iter = gameObjectList.erase(iter);	// iter = ... because it will return back the next iter value to use next
	}
	else
	{
	iter++;
	}
	}
	*/	
	
	// Call destroy() functions instead/as well as delete
	
/*	light->destroy();
	raycast->destroy();
	postProcessing->destroy();
	shaderLoader->destroy();
	//meshLoader->destroy();
	textureLoader->destroy();
	dynamicsWorld->destroy();
*/	
	if (lightTwo) {
		delete lightTwo;
		lightTwo = nullptr;
	}
	delete lightOne;
	delete raycast;
	delete gameObjects;
	delete postProcessing;
	delete shaderLoader;
	delete meshLoader;
	delete textureLoader;
	delete dynamicsWorld;

	close(SDL_window, GL_Context);

	return 0;
}
