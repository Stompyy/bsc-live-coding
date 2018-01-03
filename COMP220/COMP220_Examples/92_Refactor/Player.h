#pragma once

#include "Camera.h"
#include "gameObject.h"
//#include "EmbeddedTextureFBXLoader.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	Camera* camera;
	//EmbeddedTextureFBXLoader* FBXTexture;

	void moveForward(float value);
	void moveRight(float value);
	void jump();

	void setJumpForce(const float newJumpForce) { m_JumpForce = newJumpForce; }
	float getJumpForce() { return m_JumpForce; }

	void setMovementSpeed(const float newMovementSpeed) { m_MovementSpeed = newMovementSpeed; }
	float getMovementSpeed() { return m_MovementSpeed; }

	void updateMovement();

private:
	glm::vec3 m_DeltaPosition;
	glm::vec3 m_Up;

	float m_MovementSpeed;
	float m_JumpForce;
};
