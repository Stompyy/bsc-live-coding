#pragma once

#include <string>
#include <SDL.h>

class ErrorMessage
{
public:
	ErrorMessage();
	~ErrorMessage();

	// Display an SDL simple message box with the argument as the error message
	void showErrorMessage(const char* errorMessage) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), errorMessage, NULL); }
	void showErrorMessage(const std::string* errorMessage) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), (char*)errorMessage, NULL); }
}; 

