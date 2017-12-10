#pragma once

class PlayerController
{
public:
	PlayerController();
	~PlayerController();

	void moveForward(float value);
	void moveRight(float value);

private:
	float m_MovementSpeed;
};

