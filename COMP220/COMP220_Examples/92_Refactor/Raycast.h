#pragma once

#include <glm\glm.hpp>
#include "Camera.h"
#include "btBulletDynamicsCommon.h"

using namespace glm;

class Raycast
{
public:
	Raycast();
	~Raycast();
	void init(Camera* camera, btDynamicsWorld* dynamicsWorld);

private:
	// The ray start and end positions, in Normalized Device Coordinates
	vec4 rayStart_NDC;
	vec4 rayEnd_NDC;
};

