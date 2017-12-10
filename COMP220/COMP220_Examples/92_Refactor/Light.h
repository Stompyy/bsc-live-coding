#pragma once

#include <glm\glm.hpp>

#include "Transform.h"
#include "Material.h"

// Only using glm vector library
using namespace glm;

class Light
{
public:
	Light();
	~Light();

	Transform* transform;
	Material* colour;

	void setDirection(const float x, const float y, const float z) { m_Direction = vec3(x, y, z); }
	vec3 getDirection() { return m_Direction; }

private:
	// Do I want it to move?
	vec3 m_Direction; 
};

