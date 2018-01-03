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
	void run() { m_MovementSpeed = m_RunningSpeed; m_IsRunning = true; }
	void walk() { m_MovementSpeed = m_WalkingSpeed; m_IsRunning = false; }
	bool isRunning() { return m_IsRunning; }

	void setJumpForce(const float newJumpForce) { m_JumpForce = newJumpForce; }
	float getJumpForce() { return m_JumpForce; }

	void setRunningSpeed(const float newRunningSpeed) { m_RunningSpeed = newRunningSpeed; }
	void setWalkingSpeed(const float newWalkingSpeed) { m_WalkingSpeed = newWalkingSpeed; }
	float getRunningSpeed() { return m_RunningSpeed; }
	float getWalkingSpeed() { return m_WalkingSpeed; }

	void updateMovement();
	void updateRotation();

private:
	glm::vec3 m_DeltaPosition;
	glm::vec3 m_Up;

	float m_MovementSpeed;
	float m_WalkingSpeed;
	float m_RunningSpeed;
	float m_JumpForce;

	bool m_IsRunning;
};
