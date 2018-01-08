#include "GameManager.h"



GameManager::GameManager()
{
	m_IsRunning = true;
}


GameManager::~GameManager()
{
	destroy();
}

void GameManager::destroy()
{
}
