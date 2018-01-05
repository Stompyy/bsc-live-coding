#pragma once

#include <glm\glm.hpp>
#include <string>
#include "Camera.h"
#include "btBulletDynamicsCommon.h"

class Raycast
{
public:
	Raycast();
	~Raycast();

	// Sets the raycast at a vector dependant on the camera, and checks for collisions in the dynamics world
	void update(Camera* camera, btDynamicsWorld* dynamicsWorld);

	void destroy();

	btRigidBody* getHitObject() { return m_HitRigidBody; }

private:
	// The ray start and end positions, in Normalized Device Coordinates [-1, 1]
	glm::vec4 rayStart_NDC;
	glm::vec4 rayEnd_NDC;
	btRigidBody * m_HitRigidBody;
};

