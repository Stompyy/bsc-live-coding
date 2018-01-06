#pragma once

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class GameWindow
{
public:
	GameWindow();
	~GameWindow();

	void close();

	// Returns the SDL window
	SDL_Window* getSDLWindow() { return m_SDL_window; }

private:
	SDL_Window* m_SDL_window;
	SDL_GLContext m_GL_Context;
};

