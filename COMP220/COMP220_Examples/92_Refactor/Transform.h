#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

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

	void setRotation(const float w, const float x, const float y, const float z) { m_Rotation = quat(w, x, y, z); }
	void setRotation(const quat& newRotation) { m_Rotation = newRotation; }

	void addPosition(const vec3& deltaPosition) { m_Position += deltaPosition; }

	void updateOrientation(const vec3 lookAtPosition) { m_Rotation = conjugate(toQuat(lookAt(lookAtPosition, m_Position, m_Up))); }

	vec3& getPosition() { return m_Position; }
	vec3& getScale() { return m_Scale; }
	quat& getRotation() { return m_Rotation; }

	vec3 getFloorPosition() { return vec3(m_Position.x, 0.0f, m_Position.z); }

private:
	vec3 m_Position;
	vec3 m_Scale;
	quat m_Rotation;

	vec3 m_Up;
};

