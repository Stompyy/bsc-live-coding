#pragma once

#include <glm\glm.hpp>

using namespace glm;

class Transform
{
public:
	Transform();
	~Transform();

	void setPosition(const float x, const float y, const float z) { m_Position = vec3(x, y, z); }
	void setPosition(const vec3& newPosition) { m_Position = newPosition; }

	void setScale(const float x, const float y, const float z) { m_Scale = vec3(x, y, z); }
	void setScale(const float scale) { m_Scale = vec3(scale, scale, scale); }

	void setRotation(const float x, const float y, const float z) { m_Rotation = vec3(x, y, z); }
	void setRotation(const vec3& newRotation) { m_Rotation = newRotation; }

	vec3& getPosition() { return m_Position; }
	vec3& getRotation() { return m_Rotation; }
	vec3& getScale() { return m_Scale; }

private:
	vec3 m_Position;
	vec3 m_Scale;
	vec3 m_Rotation; //quat?
};

