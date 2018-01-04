#pragma once

#include "Camera.h"
#include "gameObject.h"
//#include "EmbeddedTextureFBXLoader.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	// The players view of the game
	Camera* camera;
	
	//EmbeddedTextureFBXLoader* FBXTexture;

	// Movement
	// Would refactor into a player controller class, but the functions need to access all inherited members, physics, transform etc, so more efficient to be here
	void moveForward();
	void moveRight();
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

	// Updates the camera and physics transform positions to the current gameObject transform position
	void updateMovement();

	// Updates the transform rotation to the current camera rotation. Only called when moved to allow a third person camera rotate to inspect the player model when stationary
	void updateRotation();

	void destroy();

private:
	glm::vec3 m_DeltaPosition;
	glm::vec3 m_Up;

	float m_MovementSpeed;
	float m_WalkingSpeed;
	float m_RunningSpeed;
	float m_JumpForce;

	bool m_IsRunning;
};
