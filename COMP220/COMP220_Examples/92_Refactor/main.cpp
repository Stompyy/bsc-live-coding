
//main.cpp - defines the entry point of the application

#include "main.h"

// #define PI = 3.14159		M_PI seems to be part of a library already

struct Model {
	Model(Mesh* mesh, GLuint* texture) : mesh(mesh), texture(texture) {}
	Mesh* mesh;
	std::vector<Mesh*>meshes;
	GLuint* texture;
};

struct SceneInfo {
	SceneInfo(MeshLoader* meshLoader, TextureLoader* textureLoader, std::vector<GameObject*> gameObjectList, PhysicsEngine* dynamicsWorld) : meshLoader(meshLoader), textureLoader(textureLoader), gameObjectList(gameObjectList), dynamicsWorld(dynamicsWorld) {}
	MeshLoader* meshLoader;
	TextureLoader* textureLoader;
	std::vector<GameObject*> gameObjectList;
	PhysicsEngine* dynamicsWorld;
};

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

SDL_GLContext GL_Init(SDL_Window* SDL_window)
{
	// 3.2 core profile version of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext GL_Context = SDL_GL_CreateContext(SDL_window);
	if (GL_Context == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL GL Create Context failed", NULL);
		SDL_DestroyWindow(SDL_window);
		SDL_Quit();
		return nullptr;
	}
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, (char*)glewGetErrorString(glewError), "GLEW Init Failed", NULL);
		return nullptr;
	}
	return GL_Context;
}

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

int main(int argc, char* args[])
{
	SDL_Window* SDL_window = SDL_Init();
	SDL_GLContext GL_Context = GL_Init(SDL_window);

	if ((SDL_window == nullptr) | (GL_Context == nullptr))  // GL_Context is not a pointer?
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "Initialisation failed", NULL);
		close(SDL_window, GL_Context);
	}

	// Create a physics simulation using bullet
	PhysicsEngine* dynamicsWorld = new PhysicsEngine();

	// Load in all needed textures
	TextureLoader* textureLoader = new TextureLoader(std::vector<std::string>{ "TrexColour.jpg", "tankColour.png", "grass.png", "archerTex1.png" });

	// Load in all needed meshes
	MeshLoader* meshLoader = new MeshLoader(std::vector<std::string>{ "Trex.FBX", "tank.FBX", "floor.FBX", "archer.FBX" });

	// Post processing class initialisation
	PostProcessing* postProcessing = new PostProcessing();
	postProcessing->setPostProcessingProgramID(LoadShaders("passThroughVert.glsl", "postCellNotCell.glsl"));
	postProcessing->setTexture0Location(glGetUniformLocation(postProcessing->getPostProcessingProgramID(), "texture0"));

	// Create an empty vector of GameObjects to store all GameObjects within the scene, inside
	std::vector<GameObject*> gameObjectList;

	/*	// Camera initialisation. To be replaced with a camera class within a player class
	Camera* camera = new Camera();
	camera->worldLocation->setPosition(0.0f, 0.5f, 15.0f);
	camera->target->setPosition(0.0f, 0.0f, 0.0f);
	camera->setProjectionMatrix(90.0f, (1000 / 800), 0.1f, 1000.0f);
	*/
	// Light initialisation
	Light* light = new Light();
	light->setDirection(0.2f, -1.0f, 0.2f);
	light->colour->setSpecularColour(0.0f, 1.0f, 0.0f);


	// Create all gameObjects and add to the GameObjects vector, and the physics simulation.
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
		vec3(0.0f, 0.0f, 0.0f),
		1.0f,
		btVector3(0.0f, 0.0f, 0.0f)
	);
	gameObjectList.push_back(trex);

	GameObject* tank3 = new GameObject();
	tank3->init(
		meshLoader->getMeshes("tank.FBX"),
		textureLoader->getTextureID("tankColour.png"),
		"lightingVert.glsl", "lightingFragTank.glsl",
		vec3(-15.0f, 200.0f, 15.0f),
		1.0f,
		btVector3(0.0f, 0.0f, 0.0f)
	);
	gameObjectList.push_back(tank3);

	GameObject* tank4 = new GameObject();
	tank4->init(
		meshLoader->getMeshes("tank.FBX"),
		textureLoader->getTextureID("tankColour.png"),
		"lightingVert.glsl", "lightingFragTank.glsl",
		vec3(15.0f, 5.0f, -15.0f),
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

	Player* player = new Player();
	player->init(
		meshLoader->getMeshes("archer.FBX"),
		textureLoader->getTextureID("archerTex1.png"),
		"lightingVert.glsl", "lightingFrag.glsl",
		vec3(10.0f, 10.0f, 0.0f),
		0.3f,
		btVector3(0.0f, 0.0f, 0.0f)
	);
	//player->FBXTexture->LoadTextureFromFBXFile("archer.FBX");
	//player->FBXTexture->loadGLTextureCall("archer.FBX");
	player->camera->setProjectionMatrix(90.0f, (1000 / 800), 0.1f, 1000.0f);
	player->transform->setScale(0.015f);
	//player->camera->worldLocation->setPosition(10.0f, 15.0f, 0.0f);
	gameObjectList.push_back(player);

	for (GameObject* gameObject : gameObjectList)
		dynamicsWorld->addRigidBody(gameObject->physics->getRigidBody());








	// Hides the mouse and takes relative position to avoid an initial snap
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_bool(SDL_ENABLE));

	// Rendering depth test
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);

	float tankRotation = 0.0f;

	// Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed
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
				player->camera->turn(SDLEvent.motion.xrel, SDLEvent.motion.yrel);
				break;

				// KEYDOWN Message, called when a key has been pressed down
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
					player->moveForward(1.0f);
					break;

				case SDLK_s:
					// Move backwards
					player->moveForward(-1.0f);
					break;

				case SDLK_a:
					// Move left
					player->moveRight(-1.0f);
					break;

				case SDLK_d:
					// Move right
					player->moveRight(1.0f);
					break;

				case SDLK_SPACE:
					// Jump
					//dynamicsWorld->getDynamicsWorld()->clearForces();
					player->jump();
					break;

				case SDLK_UP:
					// zoom in on player
					if (player->camera->getBoomLength() > 0.1f)
						player->camera->setBoomLength(player->camera->getBoomLength() - 0.1f);
					break;

				case SDLK_DOWN:
					// zoom in on player
					if (player->camera->getBoomLength() < 10.0f)
						player->camera->setBoomLength(player->camera->getBoomLength() + 0.1f);
					break;

				default:
					break;
				}
			}
		}

		player->updateMovement();

		//Input, logic with input, physics, graphics.
		tankRotation += 0.001f;
		//tank->transform->setRotation(tank->transform->getRotation().x, tankRotation, tank->transform->getRotation().z);

		postProcessing->bindFrameBuffer();

		// Advance the physics simulation
		dynamicsWorld->getDynamicsWorld()->stepSimulation(1.0f / 60.0f); // , 10);

																		 // Apply physics simulation to every GameObject
		for (GameObject* gameObject : gameObjectList)
		{
			// Update physics transform to the rigidbody transform
			gameObject->physics->setTransform(gameObject->physics->getRigidBody()->getWorldTransform());

			// Update gameObject Position to the Physics position
			btVector3 gameObjectPhysicsOrigin = gameObject->physics->getTransform().getOrigin();
			gameObject->transform->setPosition(vec3(gameObjectPhysicsOrigin.getX(), gameObjectPhysicsOrigin.getY(), gameObjectPhysicsOrigin.getZ()));

			btQuaternion objectRotation = gameObject->physics->getTransform().getRotation();

			// Update the model matrix (TRS!)
			gameObject->update();
			gameObject->preRender(player->camera, light);
			gameObject->render();
		}
		//player->FBXTexture->display();
		player->camera->update();

		// "Do all geometry things before shader"
		postProcessing->render();
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
	*/	// Call destroy() functions instead of delete
	postProcessing->destroy();
	delete postProcessing;
	//	dynamicsWorld->destroy();
	delete dynamicsWorld;

	close(SDL_window, GL_Context);

	return 0;
}
