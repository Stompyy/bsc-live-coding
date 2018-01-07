#include "GameWindow.h"


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
void GameWindow::close()
{
	SDL_GL_DeleteContext(m_GL_Context);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(m_SDL_window);

	IMG_Quit();
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();
}

GameWindow::GameWindow()
{
	// Initialise both SDL and GL
	m_SDL_window = SDL_Init();
	m_GL_Context = GL_Init(m_SDL_window);

	// If either did not initialise then exit
	if ((m_SDL_window == nullptr) | (m_GL_Context == nullptr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "Initialisation failed", NULL);
		close();
	}
}


GameWindow::~GameWindow()
{
	if (m_SDL_window) { delete m_SDL_window; m_SDL_window = nullptr; }
	if (m_GL_Context) { delete m_GL_Context; m_GL_Context = nullptr; }
}


