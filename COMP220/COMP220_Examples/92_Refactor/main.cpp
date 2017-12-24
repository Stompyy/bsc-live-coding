//main.cpp - defines the entry point of the application

#include "main.h"

// #define PI = 3.14159		M_PI seems to be part of a library already

struct Model {
	Model(Mesh* mesh, GLuint* texture) : mesh(mesh), texture(texture) {}
	Mesh* mesh;
	std::vector<Mesh*>meshes;
	GLuint* texture;
};

vec3 SphereCoordinates(float radius, float angle)
// Work in progress function to calcuate all the vertices of a sphere
{
	return vec3(0.0f) + radius * vec3(cos(angle), tan(angle), sin(angle));
}

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

	if ((SDL_window == nullptr) | (GL_Context == nullptr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "Initialisation failed", NULL);
		close(SDL_window, GL_Context);
	}

	// Create a physics simulation using bullet
	PhysicsEngine* dynamicsWorld = new PhysicsEngine();

	// Create an empty vector of GameObjects to store, all GameObjects within the scene, inside
	std::vector<GameObject*> gameObjectList;

	std::vector<std::string> texturesToLoad = { "TrexColour.jpg", "tankColour.png", "grass.png" };
	TextureLoader* textureLoader = new TextureLoader(texturesToLoad);


	std::map<std::string, std::vector<Mesh*>> meshMap;

	std::vector<std::string> meshesToLoad = { "Trex.FBX", "tank.FBX", "floor.FBX" };

	for (std::string meshName : meshesToLoad)
	{
		meshMap[meshName] = loadMeshFromFile(meshName);
	}



	// Camera initialisation. To be replaced with a camera class within a player class
	Camera* camera = new Camera();
	camera->setPosition(0.0f, 0.5f, 15.0f);
	camera->setTargetPosition(0.0f, 0.0f, 0.0f);
	camera->setProjectionMatrix(90.0f, (1000 / 800), 0.1f, 1000.0f);

	// Light initialisation
	Light* light = new Light();
	light->setDirection(0.2f, -1.0f, 0.2f);
	light->colour->setAmbientColour(1.0f, 1.0f, 1.0f);
	light->colour->setDiffuseColour(1.0f, 1.0f, 1.0f);
	light->colour->setSpecularColour(0.0f, 1.0f, 0.0f);

	// Create all gameObjects and add to the GameObjects vector, and the physics simulation.
	// Will collapse all down to a local function once finished storing the Models in a map
	GameObject* ground = new GameObject();
	ground->setMesh(meshMap["floor.FBX"]);
	ground->setDiffuseMap(textureLoader->getTextureID("grass.png"));
	ground->loadShaderProgram("textureVert.glsl", "textureFrag.glsl");
	ground->transform->setPosition(0.0f, -5.0f, 0.0f);
	ground->physics->setCollisionShapeSize(50.0f, 1.0f, 50.0f);
	ground->physics->getTransform().setIdentity();
	ground->UpdateTransformOrigin();
	ground->physics->setMass(0.0f);
	ground->physics->updateMotionState();
	gameObjectList.push_back(ground);
	dynamicsWorld->addRigidBody(ground->physics->getRigidBody());

	GameObject* trex = new GameObject();
	trex->setMesh(meshMap["Trex.FBX"]);
	trex->setDiffuseMap(textureLoader->getTextureID("TrexColour.jpg"));
	trex->loadShaderProgram("lightingVert.glsl", "lightingFrag.glsl");
	trex->transform->setPosition(0.0f, 200.0f, 0.0f);
	trex->physics->setCollisionShapeSize(0.0f, 0.0f, 0.0f);
	trex->physics->setMass(1.0f);
	trex->UpdateTransformOrigin();
	trex->physics->updateMotionState();
	gameObjectList.push_back(trex);
	dynamicsWorld->addRigidBody(trex->physics->getRigidBody());

	GameObject* tank = new GameObject();
	tank->setMesh(meshMap["tank.FBX"]);
	tank->setDiffuseMap(textureLoader->getTextureID("tankColour.png"));
	tank->loadShaderProgram("lightingVert.glsl", "lightingFragTank.glsl");
	tank->transform->setPosition(15.0f, 0.0f, 15.0f);
	tank->physics->setCollisionShapeSize(0.0f, 0.0f, 0.0f);
	tank->physics->setMass(0.1f);
	tank->UpdateTransformOrigin();
	tank->physics->updateMotionState();
	gameObjectList.push_back(tank);
	dynamicsWorld->addRigidBody(tank->physics->getRigidBody());

	GameObject* tank2 = new GameObject();
	tank2->setMesh(meshMap["tank.FBX"]);
	tank2->setDiffuseMap(textureLoader->getTextureID("tankColour.png"));
	tank2->loadShaderProgram("textureVert.glsl", "textureFrag.glsl");
	tank2->transform->setPosition(-15.0f, 0.0f, -15.0f);
	tank2->physics->setCollisionShapeSize(0.0f, 0.0f, 0.0f);
	tank2->physics->setMass(0.1f);
	tank2->UpdateTransformOrigin();
	tank2->physics->updateMotionState();
	gameObjectList.push_back(tank2);
	dynamicsWorld->addRigidBody(tank2->physics->getRigidBody());
/*
	Player* player = new Player();
	player->setMesh(meshMap["tank.FBX"]);
	player->setDiffuseMap(textureMap["tankColour.png"]); // 
	player->loadShaderProgram("textureVert.glsl", "textureFrag.glsl");
	player->transform->setPosition(0.0f, 0.0f, -15.0f);
	player->physics->setCollisionShapeSize(0.1f, 0.1f, 0.1f);
	player->physics->setMass(0.1f);
	player->UpdateTransformOrigin();
	player->physics->updateMotionState();
	gameObjectList.push_back(player);
	dynamicsWorld->addRigidBody(player->physics->getRigidBody());
*/
	// Post processing class initialisation
	PostProcessing* postProcessing = new PostProcessing();
	postProcessing->setPostProcessingProgramID(LoadShaders("passThroughVert.glsl", "postCellNotCell.glsl"));
	postProcessing->setTexture0Location(glGetUniformLocation(postProcessing->getPostProcessingProgramID(), "texture0"));
	
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
				camera->turn(SDLEvent.motion.xrel, SDLEvent.motion.yrel);
//				player->getCamera()->turn(SDLEvent.motion.xrel, SDLEvent.motion.yrel);
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
					camera->moveForward(1.0f);
//					player->moveForward(1.0f);
					break;

				case SDLK_a:
					// Move left
					camera->moveRight(-1.0f);
//					player->moveRight(-1.0f);
					break;

				case SDLK_s:
					// Move backwards
					camera->moveForward(-1.0f);
//					player->moveForward(-1.0f);
					break;

				case SDLK_d:
					// Move right
					camera->moveRight(1.0f);
//					player->moveRight(1.0f);
					break;

				case SDLK_q:
					// Add force
					// WIP
					dynamicsWorld->getDynamicsWorld()->clearForces();
					for (btRigidBody* rigidBody : dynamicsWorld->getAllRigidBodies()) {
						rigidBody->applyForce(btVector3(0.0f, 1000000000.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
						rigidBody->applyCentralForce(btVector3(0.0f, 100000000.0f, 0.0f));
						rigidBody->applyImpulse(btVector3(0.0f, 1000000000.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
					}
					break;

				case SDLK_e:
					// Add torque
					// WIP
					for (btRigidBody* rigidBody : dynamicsWorld->getAllRigidBodies()) {
						rigidBody->applyCentralForce(btVector3(100000.0f, 100000.0f, 100000.0f));
					}
					break;

				default:
					
					break;
				}
				// Update positions potentially multiple times, for each event, before rendering?
				camera->update();
//				player->getCamera()->update();

			}
		}

		//Input, logic with input, physics, graphics.
		tankRotation+=0.001f;
		tank->transform->setRotation(tank->transform->getRotation().x, tankRotation, tank->transform->getRotation().z);

		// Advance the physics simulation
		dynamicsWorld->getDynamicsWorld()->stepSimulation(1.0f / 60.0f, 10);

		// Apply physics simulation to every GameObject
		for (GameObject* object : gameObjectList)
		{
			//object->physics->getCollisionShape()->calculateLocalInertia(object->physics->getMass(), object->physics->getInertia());
			//object->transform->setPosition(object->physics->getTransform().getOrigin());
			//object->UpdateTransformOrigin();
			object->physics->setTransform(object->physics->getRigidBody()->getWorldTransform());

			btVector3 objectOrigin = object->physics->getTransform().getOrigin();
			btQuaternion objectRotation = object->physics->getTransform().getRotation();
			object->transform->setPosition(vec3(objectOrigin.getX(), objectOrigin.getY(), objectOrigin.getZ()));
//Wrong->			//object->transform->setRotation(vec3(btQuatToGlmVec3(objectRotation)));

			// Update the model matrix (TRS!)
			object->update();
		}

		
		 
		//tank->transform->setPosition(camera->getPosition());

		postProcessing->bindFrameBuffer();

		for (GameObject* object : gameObjectList)
		{
			object->preRender();
			GLuint currentProgramID = object->getShaderProgramID();

			//retrieve the shader values
			GLint viewMatrixLocation = glGetUniformLocation(currentProgramID, "viewMatrix");
			GLint projectionMatrixLocation = glGetUniformLocation(currentProgramID, "projectionMatrix");
			GLint lightDirectionLocation = glGetUniformLocation(currentProgramID, "lightDirection");
			GLint ambientLightColourLocation = glGetUniformLocation(currentProgramID, "ambientLightColour");
			GLint diffuseLightColourLocation = glGetUniformLocation(currentProgramID, "diffuseLightColour");
			GLint specularLightColourLocation = glGetUniformLocation(currentProgramID, "specularLightColour");
			GLint cameraPositionLocation = glGetUniformLocation(currentProgramID, "cameraPosition");

			//send shader values
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(camera->getViewMatrix()));
//			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(player->getCamera()->getViewMatrix()));
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(camera->getProjectionMatrix()));
//			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(player->getCamera()->getProjectionMatrix()));

			glUniform3fv(lightDirectionLocation, 1, value_ptr(light->getDirection()));
			glUniform4fv(ambientLightColourLocation, 1, value_ptr(light->colour->getAmbientColour()));
			glUniform4fv(diffuseLightColourLocation, 1, value_ptr(light->colour->getDiffuseColour()));
			glUniform4fv(specularLightColourLocation, 1, value_ptr(light->colour->getSpecularColour()));

			glUniform3fv(cameraPositionLocation, 1, value_ptr(camera->getPosition()));  //Maybe just pass in camera direction?
//			glUniform3fv(cameraPositionLocation, 1, value_ptr(player->getCamera()->getPosition()));

			object->render();
		}
		camera->update();
//		player->getCamera()->update();

		// "Do all geometry things before shader"
		postProcessing->render();
		SDL_GL_SwapWindow(SDL_window);
	}

	// Delete all Objects in reverse instantiation order
	
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
	// Call destroy() functions instead of delete
	postProcessing->destroy();
	delete postProcessing;
	delete dynamicsWorld;
	close(SDL_window, GL_Context);
	
	return 0;
}