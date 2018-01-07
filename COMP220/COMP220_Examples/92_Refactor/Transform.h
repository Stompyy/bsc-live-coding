#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

using namespace glm;

class Transform
	// Transform class. Used for anything that moves!
{
public:
	// Default constructor
	Transform();
	// Copy constructor
	Transform(Transform& transform);
	// Constructor with quat rotation argument and uniform scale
	Transform(const vec3& position, const quat& rotation, const float scale);
	// Constructor with quat rotation argument and explicit non uniform scale
	Transform(const vec3& position, const quat& rotation, const vec3& scale);
	// Constructor with readable vec3 rotation argument and uniform scale. Used for gameObject initialisation
	Transform(const vec3& position, const vec3& rotation, const float scale);

	// init matrices, called from each constructor
	void initSecondaryVariables();
	~Transform();

	// Multiple styles of constructors for readability and easier conversions from other vector libraries i.e. btVector3 - able to just construct with floats
	void setPosition(const float x, const float y, const float z) { m_Position = vec3(x, y, z); }
	void setPosition(const vec3& newPosition) { m_Position = newPosition; }

	// if x != y != z, then the result will be stretched and/or squashed along different axis.
	void setScale(const float x, const float y, const float z) { m_Scale = vec3(x, y, z); }

	// Used to preserve scale ratio
	void setScale(const float scale) { m_Scale = vec3(scale, scale, scale); }

	// All rotation in quaternions to avoid gimble lock
	void setRotation(const float w, const float x, const float y, const float z) { m_Rotation = quat(w, x, y, z); }
	void setRotation(const quat& newRotation) { m_Rotation = newRotation; }
	void setRotation(const vec3& newRotation) { m_Rotation = quat(radians(newRotation)); }
	void setRotation(const float x, const float y, const float z) { m_Rotation = quat(radians(vec3(x, y, z))); }

	// Used to move the position by deltaPosition
	void movePosition(const vec3& deltaPosition) { m_Position += deltaPosition; }
	// Move the position by x,y,z
	void movePosition(const float x, const float y, const float z) { m_Position += vec3(x, y, z); }

	// add rotation in euler angles
	void turnRotation(const float x, const float y, const float z) { m_Rotation = quat(radians(vec3(x, y, z))); }

	// Update the model matrix
	void update();

	// Update the rotation to look at a new position
	//void updateOrientation(const vec3 lookAtPosition) { m_Rotation = conjugate(toQuat(lookAt(lookAtPosition, m_Position, m_Up))); }

	vec3& getPosition() { return m_Position; }

	// Returns a shader efficient vec4 with a homogeneous w=1 for positions.
	vec4& getPositionForShader() { return vec4(m_Position, 1.0f); }

	vec3& getScale() { return m_Scale; }
	quat& getRotation() { return m_Rotation; }
	vec3 getUp() { return m_Up; }

	vec3 getPivotPosition() { return vec3(m_Position.x, 0.0f, m_Position.z); }

	mat4& getModelMatrix() { return m_ModelMatrix; }

	// Model rotation speed in euler angles per frame, default value = 0.02f
	void setRotationSpeed(const float newRotationSpeed) { m_RotationDemoSpeed = newRotationSpeed; }
	// update the rotating model demo rotation
	void updateRotatingModel() { m_RotationDemoY += m_RotationDemoSpeed; setRotation(0.0f, m_RotationDemoY, 0.0f); }
	// Returns the rotating demo model variable
	float getRotatingDemoModelY() { return m_RotationDemoY; }

private:
	vec3 m_Position;
	vec3 m_Scale;
	quat m_Rotation;

	mat4 m_TranslationMatrix;
	mat4 m_ScaleMatrix;
	mat4 m_RotationMatrix;

	mat4 m_ModelMatrix;

	vec3 m_Up;

	// Used to rotate in position, demonstrating model rotations
	float m_RotationDemoY;
	float m_RotationDemoSpeed;
};

