//main.cpp - defines the entry point of the application

#include "main.h"

void SetOpenGLAttributes()
{
	//Sets the OpenGl Version 3.2 Core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//SDL_GL_CONTEXT_CORE gives us only the newest version. Deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

int main(int argc, char* args[])
{
	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Display an error message box
		//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init failed", SDL_GetError(), NULL);
		return 1;
	}

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (window == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow failed", SDL_GetError(), NULL);
		//Close the SDL Library
		//https://wiki.libsdl.org/SDL_Quit
		SDL_Quit();
		return 1;
	}

	//Sets the OpenGL version to prepare for setting the context
	SetOpenGLAttributes();

	//Create the context for the window. Needed for rendering
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Context creation failed", SDL_GetError(), NULL); //finish this
		//Close the SDL library
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	//Start GLEW extension handler and checks ok
	glewExperimental = GL_TRUE;
	//Stores the init enum return, if needed to show an error
	GLenum glState = glewInit();
	if (glState != GLEW_OK) {
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "GLEW init failed", (char*)glewGetErrorString(glState), NULL);
		//Close the libraries in reverse init order
		// I assume that there is no need to terminate GLEW here?
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

//GLEW triangle
	//Create vertex array object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	//Identifies the vertex buffer
	GLuint vertexBuffer;
	//Generate one buffer
	glGenBuffers(1, &vertexBuffer);
	//'Talks about' 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//Gives vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


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
				//KEYDOWN Message, called when a key has been pressed down
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//Escape key
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}
		}

		//Update game and draw with OpenGL 
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

//Wtf is all this?
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,				//Attribute 0. No particular reason for 0, but must match the layout in the shader
			3,				//Size
			GL_FLOAT,		//type
			GL_FALSE,		//Normalised?
			0,				//Stride
			(void*)0		//Array buffer offset
		);

		//Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(window);

/*
		//Grab the window surface, please note we DON'T need to free the memory on this, it will be automatically collected
		//when the window is destroyed
		//https://wiki.libsdl.org/SDL_GetWindowSurface
		SDL_Surface* screenSurface= SDL_GetWindowSurface(window);

		//Fill the surface with black
		//https://wiki.libsdl.org/SDL_FillRect
		//https://wiki.libsdl.org/SDL_MapRGB
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
*/

		//Update the surface on the screen
		//https://wiki.libsdl.org/SDL_UpdateWindowSurface
		SDL_UpdateWindowSurface(window);
	}

//Destroy all libraries / memory allocations (destroy components in reverse order as they were initialised)
	//Destroy vertex buffers
	glDeleteBuffers(1, &vertexBuffer);
	//Destroy vertex arrays
	glDeleteVertexArrays(1, &VertexArrayID);
	//Destroy window context
	SDL_GL_DeleteContext(glContext);
	//Destroy the window
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//Quit SDL
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();
	//OpenGL terminate
	//glfwTerminate();

	return 0;
}