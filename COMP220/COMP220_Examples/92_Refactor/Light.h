#pragma once

#include <glm\glm.hpp>

#include "Transform.h"
#include "Material.h"

// Only using glm vectors
using namespace glm;

class Light
{
public:
	Light();
	~Light();

	Transform* transform;
	Material* colour;

	// Set the direction of the light
	void setDirection(const float x, const float y, const float z) { m_Direction = vec3(x, y, z); }
	vec3 getDirection() { return m_Direction; }

	void destroy();

private:
	vec3 m_Direction; 
};

