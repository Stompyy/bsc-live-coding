#pragma once

#include <string>
#include <SDL.h>
#include "GameManager.h"

class ErrorMessage
{
public:
	ErrorMessage();
	~ErrorMessage();

	void destroy();

	void init(GameManager* gameManager) { m_GameManager = gameManager; }

	// Display an SDL simple message box with the argument as the error message then tells gameManager to stop running
	void showErrorMessage(const char* errorMessage) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), errorMessage, NULL); m_GameManager->setIsRunning(false); }
	void showErrorMessage(const std::string* errorMessage) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), (char*)errorMessage, NULL); m_GameManager->setIsRunning(false); }

private:
	GameManager* m_GameManager;
}; 

