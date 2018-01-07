
//main.cpp - defines the entry point of the application

#include "main.h"

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

		// Draw the gameObject
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
	//// Initialise both SDL and GL
	GameWindow* gameWindow = new GameWindow();

	// Create a physics simulation using bullet
	PhysicsEngine* dynamicsWorld = new PhysicsEngine();

	// Load in all needed textures
	TextureLoader* textureLoader = new TextureLoader();
	textureLoader->init(std::vector<std::string>{"assets/grass.png", "assets/crate.png", "assets/deer.png", "assets/wolf.jpg", "assets/trex.jpg"});//"TrexColour.jpg", "tankColour.png", "grass.png", "archerTex1.png" });

	// Load in all needed meshes
	MeshLoader* meshLoader = new MeshLoader();
	meshLoader->init(std::vector<std::string>{ "assets/floor.FBX", "assets/crate.FBX", "assets/deer.FBX", "assets/wolf.FBX", "assets/trex.FBX"});//; //"Trex.FBX", "tank.FBX", "floor.FBX", "archer.FBX"});
	//"assets/cabin.FBX", 
	// Load in and label all needed shaders
	ShaderLoader* shaderLoader = new ShaderLoader();
	shaderLoader->init(std::vector<ShaderInfo*>{
			new ShaderInfo("grassShader", "textureVert.glsl", "textureFrag.glsl"),
			new ShaderInfo("assetShader", "lightingVert.glsl", "lightingFrag.glsl"),
			new ShaderInfo("brightAssetShader", "lightingVert.glsl", "lightingFragBright.glsl"),
			new ShaderInfo("darkAssetShader", "lightingVert.glsl", "lightingFragDark.glsl"),
			new ShaderInfo("postProcessingShader", "passThroughVert.glsl", "postCellNotCell.glsl")
	});

	// Post processing class initialisation
	PostProcessing* postProcessing = new PostProcessing();
	postProcessing->setPostProcessingProgramID(shaderLoader->getShaderID("postProcessingShader"));
	postProcessing->setTexture0Location(glGetUniformLocation(postProcessing->getPostProcessingProgramID(), "texture0"));

	// Load in all game objects
	GameObjectLoader* gameObjects = new GameObjectLoader();
	gameObjects->init(meshLoader, textureLoader, shaderLoader, dynamicsWorld,
		std::vector<GameObjectInfo*>{
				//					name		meshMapKey			textureMapKey		shaderMapKey		initialTransform														mass	collisionSize		
				new GameObjectInfo("ground", "assets/floor.FBX", "assets/grass.png", "grassShader",		new Transform(vec3(0.0f, -5.0f, 0.0f),	vec3(0.0f, 0.0f, 0.0f), 1.0f),		0.0f, btVector3(20.0f, 1.0f, 20.0f)),
				new GameObjectInfo("crate1", "assets/crate.FBX", "assets/crate.png", "assetShader",		new Transform(vec3(15.0f, 0.0f, 15.0f), vec3(0.0f, 10.0f, 0.0f), 0.01f),	1.0f, btVector3(1.0f, 1.0f, 1.0f)),
				new GameObjectInfo("crate2", "assets/crate.FBX", "assets/crate.png", "assetShader",		new Transform(vec3(12.0f, 0.0f, 14.0f), vec3(0.0f, -20.0f, 0.0f), 0.01f),	1.0f, btVector3(1.0f, 1.0f, 1.0f)),
				new GameObjectInfo("crate3", "assets/crate.FBX", "assets/crate.png", "assetShader",		new Transform(vec3(10.0f, 0.0f, 13.4f), vec3(0.0f, 30.0f, 0.0f), 0.01f),	1.0f, btVector3(1.0f, 1.0f, 1.0f)),
				new GameObjectInfo("crate4", "assets/crate.FBX", "assets/crate.png", "assetShader",		new Transform(vec3(7.0f, 0.0f, 12.0f),	vec3(0.0f, 45.0f, 0.0f), 0.01f),	1.0f, btVector3(1.0f, 1.0f, 1.0f)),
				new GameObjectInfo("deer1",	 "assets/deer.FBX",	 "assets/deer.png",	 "assetShader",		new Transform(vec3(5.0f, 0.0f, 5.0f),	vec3(180.0f, -20.0f, 0.0f), 0.1f),	1.0f, btVector3(1.0f, 1.0f, 1.0f)),
				new GameObjectInfo("deer2",	 "assets/deer.FBX",	 "assets/deer.png",	 "assetShader",		new Transform(vec3(3.0f, 0.0f, 3.0f),	vec3(180.0f, 175.0f, 0.0f), 0.1f),	1.0f, btVector3(1.0f, 1.0f, 1.0f)),
				new GameObjectInfo("trex",	 "assets/trex.FBX",	 "assets/trex.jpg",	 "darkAssetShader",	new Transform(vec3(0.0f, 5.0f, 0.0f),	vec3(0.0f, 15.0f, 0.0f), 1.0f),		1.0f, btVector3(1.0f, 0.0f, 1.0f))
	});
	// Add the player in seperately as it will be a GameObject child Player class instance.
	// Recommended to set gameObjectName (the first argument) here as "player" for gameObjects->getPlayer() to return the Player* player object without any arguments
	// Any other name will need the explicit gameObjects->getPlayer("yourPlayerName") to return the Player* player object
	gameObjects->addPlayer(meshLoader, textureLoader, shaderLoader, dynamicsWorld,
		new GameObjectInfo("player", "assets/wolf.FBX", "assets/wolf.jpg", "brightAssetShader", new Transform(vec3(10.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 1.0f), 0.3f, btVector3(0.85f, 0.85f, 0.85f))
	);

	// Set up camera
	gameObjects->getPlayer()->getCamera()->setProjectionMatrix(90.0f, (1000 / 800), 0.1f, 1000.0f);

	// Single raycast instance to be reused
	Raycast* raycast = new Raycast();

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
					btRigidBody* hitBody = raycast->getHitObject();
					if (hitBody)
					{
						GameObject* hitGameObject = (GameObject*)hitBody->getUserPointer();
						if (hitGameObject)
						{
							//nope...
							hitGameObject->getPhysics()->setPosition(0.0f, 20.0f, 0.0f);
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

				case SDLK_e:
					// Jump up
					gameObjects->getPlayer()->jumpUp();
					break;

				case SDLK_SPACE:
					// Jump forward
					gameObjects->getPlayer()->jumpForward();
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
		
		// Helps to demo the lights and the shaders working together, Does not provide any other real functionality
		gameObjects->getGameObject("trex")->rotateInPosition();

		// Update the player, each gameObjects physics simulation, and bind the frame buffer
		update(gameObjects, postProcessing, dynamicsWorld);
		
		// Send all gameObjects values to the shader and draw
		render(gameObjects, lightOne, lightTwo);
		
		// Update the camera MVP
		gameObjects->getPlayer()->getCamera()->update();

		// Post processing frame buffer render
		postProcessing->render();

		// Swap the SDL window to display the new image
		SDL_GL_SwapWindow(gameWindow->getSDLWindow());
	}

	// Delete all Objects in reverse instantiation order
	
	// Important! remember all this!
	//auto iter = gameObjectList.begin();
	//while (iter != gameObjectList.end())
	//{
	//if ((*iter))
	//{
	//(*iter)->destroy();
	//delete (*iter);
	//iter = gameObjectList.erase(iter);	// iter = ... because it will return back the next iter value to use next
	//}
	//else
	//{
	//iter++;
	//}
	//}
	
	
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
	if (lightOne) {
		delete lightOne;
		lightOne = nullptr;
	}
	if (raycast) {
		delete raycast;
		raycast = nullptr;
	}
	if (gameObjects) {
		delete gameObjects;
		gameObjects = nullptr;
	}
	if (postProcessing) {
		delete postProcessing;
		postProcessing = nullptr;
	}
	if (shaderLoader) {
		delete shaderLoader;
		shaderLoader = nullptr;
	}
	if (meshLoader) {
		delete meshLoader;
		meshLoader = nullptr;
	}
	if (textureLoader) {
		delete textureLoader;
		textureLoader = nullptr;
	}
	if (dynamicsWorld) {
		delete dynamicsWorld;
		dynamicsWorld = nullptr;
	}

	gameWindow->close();

	return 0;
}
