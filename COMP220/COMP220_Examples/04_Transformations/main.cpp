//main.cpp - defines the entry point of the application

#include "main.h"
#include "vertex.h"

#define PI = 3.14159

vec3 SphereCoordinates(float radius, float angle)
{
	return vec3(0.0f) + radius * vec3(cos(angle), tan(angle), sin(angle));
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
	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
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

	


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		// 2 parrallel triangles
//		-1.0f, -1.0f, 0.0f,
//		1.0f, -1.0f, 0.0f,
//		0.0f,  1.0f, 0.0f,
//		-1.0f, -1.0f, -1.0f,
//		1.0f, -1.0f, -1.0f,
//		0.0f,  1.0f, -1.0f,

// Room
		// Floor
		2.0f,	-1.0f,	-10.0f,
		-2.0f,	-1.0f,	-10.0f,
		-2.0f,	-1.0f,	-4.0f,

		2.0f,	-1.0f,	-4.0f,
		2.0f,	-1.0f,	-10.0f,
		-2.0f,	-1.0f,	-4.0f,

		// ceiling
		-2.0f,	2.0f,	-4.0f,
		-2.0f,	2.0f,	-10.0f,
		2.0f,	2.0f,	-10.0f,

		-2.0f,	2.0f,	-4.0f,
		2.0f,	2.0f,	-10.0f,
		2.0f,	2.0f,	-4.0f,

		// Left wall
		-2.0f,	-1.0f,	-4.0f,
		-2.0f,	-1.0f,	-10.0f,
		-2.0f,	2.0f,	-4.0f,

		-2.0f,	-1.0f,	-10.0f,
		-2.0f,	2.0f,	-10.0f,
		-2.0f,	2.0f,	-4.0f,

		// Right wall
		2.0f,	2.0f,	-4.0f,
		2.0f,	-1.0f,	-10.0f,
		2.0f,	-1.0f,	-4.0f,

		2.0f,	2.0f,	-4.0f,
		2.0f,	2.0f,	-10.0f,
		2.0f,	-1.0f,	-10.0f,

		// Back wall
		2.0f,	-1.0f,	-10.0f,
		-2.0f,	2.0f,	-10.0f,
		-2.0f,	-1.0f,	-10.0f,

		2.0f,	-1.0f,	-10.0f,
		2.0f,	2.0f,	-10.0f,
		-2.0f,	2.0f,	-10.0f,

// Corridor
		// Floor
		1.0f,	-1.0f,	-4.0f,
		-1.0f,	-1.0f,	-4.0f,
		-1.0f,	-1.0f,	0.0f,

		1.0f,	-1.0f,	0.0f,
		1.0f,	-1.0f,	-4.0f,
		-1.0f,	-1.0f,	0.0f,

		// ceiling
		-1.0f,	1.0f,	0.0f,
		-1.0f,	1.0f,	-4.0f,
		1.0f,	1.0f,	-4.0f,

		-1.0f,	1.0f,	0.0f,
		1.0f,	1.0f,	-4.0f,
		1.0f,	1.0f,	0.0f,

		// Left wall
		-1.0f,	-1.0f,	0.0f,
		-1.0f,	-1.0f,	-4.0f,
		-1.0f,	1.0f,	0.0f,

		-1.0f,	-1.0f,	-4.0f,
		-1.0f,	1.0f,	-4.0f,
		-1.0f,	1.0f,	0.0f,

		// Right wall
		1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,	-4.0f,
		1.0f,	-1.0f,	0.0f,

		1.0f,	1.0f,	0.0f,
		1.0f,	1.0f,	-4.0f,
		1.0f,	-1.0f,	-4.0f,

		};

/*
	static const GLfloat g_vertex_buffer_data[] = {
		0.0f,	0.0f,	0.0f,
	};

	for (int i = 0; i < 360; i + 10)
	{
		g_vertex_buffer_data[i] = SphereCoordinates(10.0f, (float)i).x, SphereCoordinates(10.0f, (float)i).y, SphereCoordinates(10.0f, (float)i).z,
	}
*/
	// Number of vertices
	int NumberOfVertices = 54;
/*
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
*/
	Vertex mapVertices[] = {// Floor
		{0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f,	0.0f },
		{0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f,	0.0f },
		{0.0f, 1.0f, 0.5f,	0.0f, 1.0f, 0.5f,	1.0f,	1.0f },

		{0.0f, 1.0f, 0.5f,	0.0f, 1.0f, 0.5f,	0.0f,	0.0f },
		{0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f,	0.0f },
		{0.0f, 1.0f, 0.5f,	0.0f, 1.0f, 0.5f,	0.0f,	0.0f },

		// Ceiling
		{0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f,	0.0f },
		{0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f,	0.0f },
		{0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f,	0.0f },

		{0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f,	0.0f },
		{0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f,	0.0f },
		{0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f, 	0.0f,	0.0f },

		// Left wall
		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },

		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f,	0.0f },

		// Right wall
		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },

		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },

		// Back wall
		{0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f, 	0.0f,	0.0f },
		{0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f, 	0.0f,	0.0f },
		{0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f, 	0.0f,	0.0f },

		{0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f, 	0.0f,	0.0f },
		{0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f, 	0.0f,	0.0f },
		{0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f, 	0.0f,	0.0f },

		// Corridor
		// Floor
		{0.0f, 1.0f, 0.5f,	0.0f, 1.0f, 0.5f, 	0.0f,	0.0f },
		{0.0f, 1.0f, 0.5f,	0.0f, 1.0f, 0.5f, 	0.0f,	0.0f },
		{0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f, 	0.0f,	0.0f },

		{0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{0.0f, 1.0f, 0.5f,	0.0f, 1.0f, 0.5f, 	0.0f,	0.0f },
		{0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f, 	0.0f,	0.0f },

		// Ceiling
		{0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f, 	0.0f,	0.0f },
		{0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f, 	0.0f,	0.0f },
		{0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f, 	0.0f,	0.0f },

		{0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f, 	0.0f,	0.0f },
		{0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f, 	0.0f,	0.0f },
		{0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f, 	0.0f,	0.0f },

		// Right wall
		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },

		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },

		// Left wall
		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },

		{1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
		{1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 	0.0f,	0.0f },
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, NumberOfVertices * sizeof(Vertex), mapVertices, GL_STATIC_DRAW);
	
	unsigned int triangleIndices[] =
	{
		0,1,2,
		2,0,3
	};
	
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), triangleIndices, GL_STATIC_DRAW);


	// Colours
	static const GLfloat g_vertex_buffer_colour[] = {
		// Rainbow effect
/*		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f,  0.0f, 1.0f,
*/
// Room
		// Floor
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.5f,

		0.0f, 1.0f, 0.5f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.5f,

		// Ceiling
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		// Left wall
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		// Right wall
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		// Back wall
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

// Corridor
		// Floor
		0.0f, 1.0f, 0.5f,
		0.0f, 1.0f, 0.5f,
		0.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.5f,
		0.0f, 1.0f, 0.0f,

		// Ceiling
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		// Right wall
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		// Left wall
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		
	};

	// This will identify our colour buffer
	GLuint colourbuffer;
	// Generate 1 buffer, put the resulting identifier in colourbuffer
	glGenBuffers(1, &colourbuffer);
	// The following commands will talk about our 'colourbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
	// Give our colours to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_colour), g_vertex_buffer_colour, GL_STATIC_DRAW);
	//
	//glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), V, GL_STATIC_DRAW);
	//
	//Colour:
	//glVertexAttribPointer(
	//	1,								glEnableVertexAttributeArray(1)
	//	4,								Colour has 4 elements
	//	GL_FLOAT,						size of data lump in memory
	//	GL_FALSE,						always...
	//	sizeof(Vertex),					stride
	//	(void*)(3 * sizeof(float))		offset
	//	);

	GLuint textureID = loadTextureFromFile("brick.jpg");
	/*
	
	*/
	

	//Initiate variables
	vec3 trianglePosition = vec3(1.0f, 0.0f, 0.0f);
	vec3 triangleScale = vec3(1.0f, 1.0f, 1.0f);
	vec3 triangleRotation = vec3(0.0f, 0.0f, 0.0f);

	// Set the matrices according to the vec3s above
	mat4 translationMatrix = translate(trianglePosition);
	mat4 scaleMatrix = scale(triangleScale);
	mat4 rotationMatrix = rotate(triangleRotation.x, vec3(1.0f, 0.0f, 0.0f))
						* rotate(triangleRotation.y, vec3(0.0f, 1.0f, 0.0f))
						* rotate(triangleRotation.z, vec3(0.0f, 0.0f, 1.0f));

	// TRS!!!
	mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;


	vec3 cameraPosition = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, -10.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

	// Perspective( fieldOfView, aspectRatio, nearClip, farClip )
	mat4 projectionMatrix = perspective(radians(90.0f), float(800 / 600), 0.1f, 100.0f);


	GLuint programID = LoadShaders("textureVert.glsl", "textureFrag.glsl");

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


	vec3 DeltaPosition;
	float CameraDistance = (float)(cameraTarget - cameraPosition).length();
	float TurnDegreesFromOriginX = -90.0f;
	float TurnDegreesFromOriginY = 0.0f;

	float ControlSensitivity = 0.04f;

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
				TurnDegreesFromOriginY += -ev.motion.yrel / 200.0f;
				// Clamp Y to avoid gimble lock as tan tends towards infinity
				if		(TurnDegreesFromOriginY > 85.0f)	TurnDegreesFromOriginY = 85.0f;
				else if (TurnDegreesFromOriginY < -85.0f)	TurnDegreesFromOriginY = -85.0f;
				
				// Move camera lookatpoint to a trigonometry calculated position, CameraDistance far away, relative to the camera position
				cameraTarget = cameraPosition + CameraDistance * vec3(cos(TurnDegreesFromOriginX), tan(TurnDegreesFromOriginY), sin(TurnDegreesFromOriginX));
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
					DeltaPosition = normalize(cameraTarget - cameraPosition) * 0.1f;
					break;

				case SDLK_a:
					// Move left
					DeltaPosition = -cross((normalize(cameraTarget - cameraPosition)), cameraUp) * ControlSensitivity;
					break;

				case SDLK_s:
					// Move backwards
					DeltaPosition = -normalize(cameraTarget - cameraPosition) * 0.1f;
					break;

				case SDLK_d:
					// Move right
					DeltaPosition = cross((normalize(cameraTarget - cameraPosition)), cameraUp) * ControlSensitivity;
					break;

				default:
					DeltaPosition = vec3(0.0f);

				}
				// Update positions potentially multiple times, for each event, before rendering
				cameraPosition += vec3(DeltaPosition.x, 0.0f, DeltaPosition.z);
				cameraTarget += vec3(DeltaPosition.x, 0.0f, DeltaPosition.z);
			}
		}

		viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// "Do all geometry things before shader"
		// Activate the texture
		glActiveTexture(GL_TEXTURE0);
		// Bind the texture, bind the buffers
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, elementBuffer);

		// Use the program
		glUseProgram(programID);

		// Send everything across
		glUniform4fv(fragColourLocation, 1, fragColour);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
		// No uniform texture, tell the shader that the texture is in slot 0 - because of GL_TEXTURE0 above
		glUniform1i(textureLocation, 0);


		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		//2nd attribute buffer : colours
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, NumberOfVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(window);

	}

	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colourbuffer);
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