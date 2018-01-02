#pragma once

#include <glm\gtx\transform.hpp>
#include "Camera.h"
#include "gameObject.h"
#include "PlayerController.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	Camera* camera;

	void moveForward(float value);
	void moveRight(float value);

	void setJumpForce(const float newJumpForce) { m_JumpForce = newJumpForce; }
	float getJumpForce() { return m_JumpForce; }

	void setMovementSpeed(const float newMovementSpeed) { m_MovementSpeed = newMovementSpeed; }
	float getMovementSpeed() { return m_MovementSpeed; }

private:
	glm::vec3 m_Up;
	glm::vec3 m_DeltaPosition;

	float m_MovementSpeed;
	float m_JumpForce;
};

