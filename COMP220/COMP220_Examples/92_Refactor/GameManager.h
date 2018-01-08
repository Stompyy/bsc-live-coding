#pragma once
class GameManager
{
public:
	GameManager();
	~GameManager();
	
	void destroy();

	// Sets the gameManager's isRunning state
	void setIsRunning(const bool isRunning) { m_IsRunning = isRunning; }

	// Get whether the game is running
	bool getIsRunning() { return m_IsRunning; }

private:
	bool m_IsRunning;
};

