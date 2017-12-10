#pragma once

#include <glm\gtx\transform.hpp>
#include "gameObject.h"
#include "Camera.h"
#include "PlayerController.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void moveForward(float value);
	void moveRight(float value);

	Camera* getCamera() { return m_Camera; }

private:
	Camera* m_Camera;

	vec3 m_DeltaPosition;
	float m_MovementSpeed;
};

