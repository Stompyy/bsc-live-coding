
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
	// Initialise both SDL and GL
	GameWindow* gameWindow = new GameWindow();

	// GameManager class handles if the game is running
	GameManager* gameManager = new GameManager();

	// SDL error message instance to be used by all classes after SDL initialisation. Can tell the gameManager to stop the game running.
	ErrorMessage* errorMessage = new ErrorMessage();
	errorMessage->init(gameManager);

	// Create a physics simulation using bullet
	PhysicsEngine* dynamicsWorld = new PhysicsEngine();

	// Load in all needed textures
	TextureLoader* textureLoader = new TextureLoader();
	textureLoader->init(errorMessage, std::vector<std::string>{ "assets/grass.png", "assets/crate.png", "assets/deer.png", "assets/wolf.jpg", "assets/trex.jpg" });

	// Load in all needed meshes
	MeshLoader* meshLoader = new MeshLoader();
	meshLoader->init(errorMessage, std::vector<std::string>{ "assets/floor.FBX", "assets/crate.FBX", "assets/deer.FBX", "assets/wolf.FBX", "assets/trex.FBX" });
	
	// Load in and label all needed shaders
	ShaderLoader* shaderLoader = new ShaderLoader();
	shaderLoader->init(errorMessage, std::vector<ShaderInfo*>{
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
	gameObjects->init(meshLoader, textureLoader, shaderLoader, dynamicsWorld, errorMessage,
		std::vector<GameObjectInfo*>{
			//					name		meshMapKey			textureMapKey		shaderMapKey		initialTransform														mass	collisionSize				activePhysics
			new GameObjectInfo("ground", "assets/floor.FBX", "assets/grass.png", "grassShader",		new Transform(vec3(0.0f, -5.0f, 0.0f),	vec3(0.0f, 0.0f, 0.0f),		1.0f),	0.0f, btVector3(30.0f, 1.0f, 30.0f),false),
			new GameObjectInfo("crate1", "assets/crate.FBX", "assets/crate.png", "assetShader",		new Transform(vec3(15.0f, 0.0f, 15.0f), vec3(0.0f, 10.0f, 0.0f),	0.01f), 1.0f, btVector3(1.0f, 1.0f, 1.0f),	true),
			new GameObjectInfo("crate2", "assets/crate.FBX", "assets/crate.png", "assetShader",		new Transform(vec3(12.0f, 0.0f, 14.0f), vec3(0.0f, -20.0f, 0.0f),	0.01f), 1.0f, btVector3(1.0f, 1.0f, 1.0f),	true),
			new GameObjectInfo("crate3", "assets/crate.FBX", "assets/crate.png", "assetShader",		new Transform(vec3(10.0f, 0.0f, 13.4f), vec3(0.0f, 30.0f, 0.0f),	0.01f), 1.0f, btVector3(1.0f, 1.0f, 1.0f),	true),
			new GameObjectInfo("crate4", "assets/crate.FBX", "assets/crate.png", "assetShader",		new Transform(vec3(7.0f, 0.0f, 12.0f),	vec3(0.0f, 45.0f, 0.0f),	0.01f), 1.0f, btVector3(1.0f, 1.0f, 1.0f),	true),
			new GameObjectInfo("deer1",	 "assets/deer.FBX",	 "assets/deer.png",	 "assetShader",		new Transform(vec3(5.0f, 0.0f, 5.0f),	vec3(180.0f, -20.0f, 0.0f), 0.1f),	1.0f, btVector3(1.0f, 1.0f, 1.0f),	true),
			new GameObjectInfo("deer2",	 "assets/deer.FBX",	 "assets/deer.png",	 "assetShader",		new Transform(vec3(3.0f, 0.0f, 3.0f),	vec3(180.0f, 175.0f, 0.0f), 0.1f),	1.0f, btVector3(1.0f, 1.0f, 1.0f),	true),
			new GameObjectInfo("trex",	 "assets/trex.FBX",	 "assets/trex.jpg",	 "darkAssetShader", new Transform(vec3(0.0f, 5.0f, 0.0f),	vec3(0.0f, 15.0f, 0.0f),	1.0f),	10.0f,btVector3(1.0f, 0.0f, 1.0f),	true)
	});

	// Add the player in seperately as it will be a GameObject child Player class instance.
	// Recommended to set gameObjectName (the first argument) here as "player" for gameObjects->getPlayer() to return the Player* player object without any arguments
	// Any other name will need the explicit gameObjects->getPlayer("yourPlayerName") to return the Player* player object
	gameObjects->addPlayer(meshLoader, textureLoader, shaderLoader, dynamicsWorld,
		new GameObjectInfo("player",	"assets/wolf.FBX",	"assets/wolf.jpg",	"brightAssetShader",new Transform(vec3(10.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),		1.0f),	0.3f, btVector3(0.85f, 0.85f, 0.85f), true)
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

	// Controls class handles all player input and passes actions to the gameObjects
	Controls* controls = new Controls();
	controls->init(gameManager, gameObjects, raycast, dynamicsWorld);

	// Hides the mouse and sets relative position to avoid an initial snap
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_bool(SDL_ENABLE));

	// Enable rendering depth test
	glEnable(GL_DEPTH_TEST);

	// Event loop, we will loop until running is set to false, if escape has been pressed or window is closed
	while (gameManager->getIsRunning())
	{
		// Mantra: input, logic with input, physics, graphics.

		// Get input from SDL_Event, Controls class processes each control and passes on to gameObjects
		controls->update();

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
	// Sanity check... if exists, delete, then set to nullptr. Avoids memory leak on the heap. https://stackoverflow.com/questions/3559412/how-to-store-different-data-types-in-one-list-c
	if (controls)		{ delete controls;			controls = nullptr; }
	if (lightTwo)		{ delete lightTwo;			lightTwo = nullptr; }
	if (lightOne)		{ delete lightOne;			lightOne = nullptr; }
	if (raycast)		{ delete raycast;			raycast = nullptr; }
	if (gameObjects)	{ delete gameObjects;		gameObjects = nullptr; }
	if (postProcessing) { delete postProcessing;	postProcessing = nullptr; }
	if (shaderLoader)	{ delete shaderLoader;		shaderLoader = nullptr; }
	if (meshLoader)		{ delete meshLoader;		meshLoader = nullptr; }
	if (textureLoader)	{ delete textureLoader;		textureLoader = nullptr; }
	if (dynamicsWorld)	{ delete dynamicsWorld;		dynamicsWorld = nullptr; }
	if (errorMessage)	{ delete errorMessage;		errorMessage = nullptr; }
	if (gameManager)	{ delete gameManager;		gameManager = nullptr; }
	if (gameWindow)		gameWindow->close();

	return 0;
}
