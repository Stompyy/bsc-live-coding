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

	// Returns the Transform member
	Transform* getTransform() { return m_Transform; }

	// Returns the Material member
	Material* getColour() { return m_Colour; }

	// Set the direction of the light
	void setDirection(const float x, const float y, const float z) { m_Direction = vec3(x, y, z); }

	// Returns a shader efficient vec4 with a homogeneous w=0 for directions.
	vec4& getDirectionForShader() { return vec4(m_Direction, 0.0f); }

	void destroy();

private:
	Transform* m_Transform;
	Material* m_Colour;

	vec3 m_Direction; 
};

