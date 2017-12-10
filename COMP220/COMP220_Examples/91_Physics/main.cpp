//main.cpp - defines the entry point of the application

#include "main.h"
#include "vertex.h"

// #define PI = 3.14159		M_PI seems to be part of a library already

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


int main(int argc, char* args[])
{
	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Display an error message box
		//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_Init failed", NULL);
		return 1;
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (window == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_CreateWindow failed", NULL);
		//Close the SDL Library
		//https://wiki.libsdl.org/SDL_Quit
		SDL_Quit();
		return 1;
	}

	//lets ask for a 3.2 core profile version of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext GL_Context = SDL_GL_CreateContext(window);
	if (GL_Context == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL GL Create Context failed", NULL);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, (char*)glewGetErrorString(glewError), "GLEW Init Failed", NULL);
	}

	

	std::vector<Mesh*> meshes;
	loadMeshFromFile("Trex.FBX", meshes);

	//loadMeshFromFile("tank.FBX", meshes);



	GLuint textureID = loadTextureFromFile("TrexColour.jpg");
	
	//Initiate variables
	vec3 modelPosition = vec3(0.0f, 10.0f, 0.0f);
	vec3 modelScale = vec3(1.0f, 1.0f, 1.0f);
	vec3 modelRotation = vec3(0.0f, -2.0f, 0.0f);

	// Set the matrices according to the vec3s above
	mat4 translationMatrix = translate(modelPosition);
	mat4 scaleMatrix = scale(modelScale);
	mat4 rotationMatrix = rotate(modelRotation.x, vec3(1.0f, 0.0f, 0.0f))
						* rotate(modelRotation.y, vec3(0.0f, 1.0f, 0.0f))
						* rotate(modelRotation.z, vec3(0.0f, 0.0f, 1.0f));

	// TRS!!!
	mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;


	vec3 cameraPosition = vec3(0.0f, 4.0f, -20.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

	// Perspective( fieldOfView, aspectRatio, nearClip, farClip )
	mat4 projectionMatrix = perspective(radians(90.0f), float(1000 / 800), 0.1f, 100.0f);

	//Need a light class

	// Light
	vec4 ambientLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 lightDirection = vec3(-1.0f, -1.0f, -1.0f);
	vec4 diffuseLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);  //Question. What is the material light stuff compared to this?
	vec4 specularLightColour = vec4(0.0f, 1.0f, 0.0f, 1.0f);

	// Material
	vec4 ambientMaterialColour = vec4(0.4f, 0.4f, 0.4f, 1.0f);
	vec4 diffuseMaterialColour = vec4(0.6f, 0.6f, 0.6f, 1.0f);
	vec4 specularMaterialColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float specularPower = 25.0f;	// Good default value - but experiment!

	// Colour buffer (texture)
	GLuint colourBufferID = CreateTexture(1000, 800);

	// Create depth buffer
	GLuint depthRenderBufferID;
	glGenRenderbuffers(1, &depthRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1000, 800);

	// Create frame buffer
	GLuint frameBufferID;
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colourBufferID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Unable to create frame buffer for post processing", "Frame buffer error", NULL);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Create screen aligned quad
	GLfloat screenVerts[] =
	{
		-1, -1,
		1, -1,
		-1, 1,
		1, 1
	};

	GLuint screenQuadVBOID;
	glGenBuffers(1, &screenQuadVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), screenVerts, GL_STATIC_DRAW);

	GLuint screenVAO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint postProcessingProgramID = LoadShaders("passThroughVert.glsl", "postCellNotCell.glsl");
	GLint texture0Location = glGetUniformLocation(postProcessingProgramID, "texture0");

	GLuint programID = LoadShaders("lightingVert.glsl", "lightingFrag.glsl");

	GLint fragColourLocation=glGetUniformLocation(programID, "fragColour");
	if (fragColourLocation < 0)
	{
		printf("Unable to find %s uniform", "fragColour");
	}

	static const GLfloat fragColour[] = { 1.0f,1.0f,0.0f,1.0f };

	// Probably want some error checking for these below
	GLint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	GLint viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");
	GLint textureLocation = glGetUniformLocation(programID, "baseTexture");
	GLint cameraPositionLocation = glGetUniformLocation(programID, "cameraPosition");

	GLint lightDirectionLocation = glGetUniformLocation(programID, "lightDirection");
	GLint ambientLightColourLocation = glGetUniformLocation(programID, "ambientLightColour");
	GLint diffuseLightColourLocation = glGetUniformLocation(programID, "diffuseLightColour");
	GLint specularLightColourLocation = glGetUniformLocation(programID, "specularLightColour");

	GLint ambientMaterialColourLocation = glGetUniformLocation(programID, "ambientMaterialColour");
	GLint diffuseMaterialColourLocation = glGetUniformLocation(programID, "diffuseMaterialColour");
	GLint specularMaterialColourLocation = glGetUniformLocation(programID, "specularMaterialColour");
	GLint specularPowerLocation = glGetUniformLocation(programID, "specularPower");



	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// Note: There are many different bt...Shapes, spheres, meshes etc.
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(2.), btScalar(50.)));

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -5, 0));
	
	btScalar groundMass(0.);
	btVector3 localInertia(0, 0, 0);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	// Note: next line associates shape with rigid body., Just refer to rigidbody from now on. Forget the bt shape
	btRigidBody::btRigidBodyConstructionInfo rbInfo(groundMass, myMotionState, groundShape, localInertia);
	btRigidBody* groundRigidBody = new btRigidBody(rbInfo);

	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(groundRigidBody);

	btCollisionShape* trexCollisionShape = new btBoxShape(btVector3(2, 2, 2));

	btTransform trexTransform;
	trexTransform.setIdentity();
	trexTransform.setOrigin(btVector3(modelPosition.x, modelPosition.y, modelPosition.z));

	// Get better weight...
	btScalar trexMass(1.0f);
	btVector3 trexInertia(0, 0, 0);
	trexCollisionShape->calculateLocalInertia(trexMass, trexInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* trexMotionState = new btDefaultMotionState(trexTransform);
	// Note: next line associates shape with rigid body., Just refer to rigidbody from now on. Forget the bt shape
	btRigidBody::btRigidBodyConstructionInfo trexRbInfo(trexMass, trexMotionState, trexCollisionShape, trexInertia);
	btRigidBody* trexRigidBody = new btRigidBody(trexRbInfo);

	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(trexRigidBody);



	vec3 DeltaPosition;
	float CameraDistance = (float)(cameraTarget - cameraPosition).length();
	float TurnDegreesFromOriginX = 90.0f;
	float TurnDegreesFromOriginY = 0.0f;

	float ControlSensitivity = 0.2f;

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_bool(SDL_ENABLE));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed
	bool running = true;
	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;
	while (running)
	{
		//Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&ev))
		{
			//Switch case for every message we are intereted in
			switch (ev.type)
			{
				//QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;

				//MOUSEMOTION Message, called when the mouse has been moved
			case SDL_MOUSEMOTION:
				// Adjust viewing angle variables based on mouse movement
				TurnDegreesFromOriginX +=  ev.motion.xrel / 200.0f;
				TurnDegreesFromOriginY += -tan(ev.motion.yrel / 200.0f);
				// Clamp Y to avoid gimble lock as tan tends towards infinity
				if		(TurnDegreesFromOriginY > 0.95f)	TurnDegreesFromOriginY = 0.95f;
				else if (TurnDegreesFromOriginY < -0.95f)	TurnDegreesFromOriginY = -0.95f;
				
				// Move camera lookatpoint to a trigonometry calculated position, CameraDistance far away, relative to the camera position
				cameraTarget = cameraPosition + CameraDistance * vec3(cos(TurnDegreesFromOriginX), TurnDegreesFromOriginY, sin(TurnDegreesFromOriginX));
				break;

				//KEYDOWN Message, called when a key has been pressed down
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//Escape key
				case SDLK_ESCAPE:
					running = false;
					break;

				case SDLK_w:
					// Move Forward
					DeltaPosition = normalize(cameraTarget - cameraPosition) * ControlSensitivity;
					break;

				case SDLK_a:
					// Move left
					DeltaPosition = -cross((normalize(cameraTarget - cameraPosition)), cameraUp) * ControlSensitivity;
					break;

				case SDLK_s:
					// Move backwards
					DeltaPosition = -normalize(cameraTarget - cameraPosition) * ControlSensitivity;
					break;

				case SDLK_d:
					// Move right
					DeltaPosition = cross((normalize(cameraTarget - cameraPosition)), cameraUp) * ControlSensitivity;
					break;

				case SDLK_q:
					// Add force
					trexRigidBody->applyCentralForce(btVector3(-10.0f, -10.0f, -10.0f));
					break;

				case SDLK_e:
					//Add torque
					trexRigidBody->applyTorque(btVector3(-10.0f, -10.0f, -10.0f));
					break;

				default:
					DeltaPosition = vec3(0.0f);
					trexMass = 1.0f;
					trexInertia = btVector3(0.0f, 0.0f, 0.0f);

				}
				// Update positions potentially multiple times, for each event, before rendering
				cameraPosition += vec3(DeltaPosition.x, 0.0f, DeltaPosition.z);
				cameraTarget += vec3(DeltaPosition.x, 0.0f, DeltaPosition.z);
				
				
			}
		}

		viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

		//Input, logic with input, physics, graphics.
		dynamicsWorld->stepSimulation(1.0f / 60.0f, 10);
		
		trexCollisionShape->calculateLocalInertia(trexMass, trexInertia);
		
		// Reuse the trextransform
		trexTransform = trexRigidBody->getWorldTransform();

		
		
		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		//trexMotionState = new btDefaultMotionState(trexTransform);
		
		// Note: next line associates shape with rigid body., Just refer to rigidbody from now on. Forget the bt shape
		//btRigidBody::btRigidBodyConstructionInfo trexRbInfo(trexMass, trexMotionState, trexCollisionShape, trexInertia);
		//trexRigidBody = new btRigidBody(trexRbInfo);


		
		// TODO trexTransform.getRotation quart, convert to glm rotation matrix, 
		// get rotat convert to glm.quart calculate model matrix
		btVector3 trexOrigin = trexTransform.getOrigin();
		btQuaternion trexRotation = trexTransform.getRotation();

		// note: Not trexOrigin.x, trexOrigin.y, trexOrigin.z ???
		modelPosition = vec3(trexOrigin.getX(), trexOrigin.getY(), trexOrigin.getZ());
		
		modelRotation = vec3(btQuatToGlmVec3(trexRotation));
		//quat GLMRot = quat(trexRotation.getW(), trexRotation.getX(), trexRotation.getY(), trexRotation.getZ());
		
		// Update the model matrix every tick
		translationMatrix = translate(modelPosition);
		scaleMatrix = scale(modelScale);
		rotationMatrix = rotate(modelRotation.x, vec3(1.0f, 0.0f, 0.0f))
			* rotate(modelRotation.y, vec3(0.0f, 1.0f, 0.0f))
			* rotate(modelRotation.z, vec3(0.0f, 0.0f, 1.0f));

		
		// TRS!!!
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		


		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);			//Maybe this bit delete? doing later

		// "Do all geometry things before shader"

		// Activate the texture
		glActiveTexture(GL_TEXTURE0);
		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, textureID);		
		
		// Use the program
		glUseProgram(programID);

		// Send everything across
		glUniform4fv(fragColourLocation, 1, fragColour);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));

		glUniform3fv(cameraPositionLocation, 1, value_ptr(cameraPosition));	//Maybe just pass in camera direction?

		// No uniform texture, tell the shader that the texture is in slot 0 - because of GL_TEXTURE0 above
		glUniform1i(textureLocation, 0);
		glUniform3fv(lightDirectionLocation, 1, value_ptr(lightDirection));
		glUniform4fv(ambientLightColourLocation, 1, value_ptr(ambientLightColour));
		glUniform4fv(diffuseLightColourLocation, 1, value_ptr(diffuseLightColour));
		glUniform4fv(specularLightColourLocation, 1, value_ptr(specularLightColour));


		glUniform4fv(ambientMaterialColourLocation, 1, value_ptr(ambientMaterialColour));
		glUniform4fv(diffuseMaterialColourLocation, 1, value_ptr(diffuseMaterialColour));
		glUniform4fv(specularMaterialColourLocation, 1, value_ptr(specularMaterialColour));
		glUniform1f(specularPowerLocation, specularPower);

		for (Mesh* currentMesh : meshes)		// New notation...? for (Mesh* currentMesh = ...; currentMesh < ...; currentMesh++)
		{
			currentMesh->render();
		}

		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind post processing shaders
		glUseProgram(postProcessingProgramID);

		// Activate texture unit 0 for the colour buffer
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colourBufferID);
		glUniform1i(texture0Location, 0);

		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//glDisableVertexAttribArray(0);		// what was this again?

		SDL_GL_SwapWindow(window);

	}
	
	// Important! remember all this!
	auto iter = meshes.begin();
	while (iter != meshes.end())
	{
		if ((*iter))
		{
			(*iter)->destroy();
			delete (*iter);
			iter = meshes.erase(iter);	// iter = ... because it will return back the next iter value to use next
		}
		else
		{
			iter++;
		}
	}

	dynamicsWorld->removeRigidBody(trexRigidBody);
	delete trexCollisionShape;
	delete trexRigidBody->getMotionState();
	delete trexRigidBody;
	
	dynamicsWorld->removeRigidBody(groundRigidBody);

	// Delete ground shape
	delete groundShape;
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;


	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;


	glDeleteProgram(postProcessingProgramID);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &screenQuadVBOID);

	glDeleteFramebuffers(1, &frameBufferID);
	glDeleteRenderbuffers(1, &depthRenderBufferID);
	glDeleteTextures(1, &colourBufferID);

	meshes.clear();

	glDeleteTextures(1, &textureID);
	glDeleteProgram(programID);

	SDL_GL_DeleteContext(GL_Context);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);

	IMG_Quit();
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}