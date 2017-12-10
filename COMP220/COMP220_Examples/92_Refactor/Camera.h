#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

// Only using glm vector library
using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();

	// Transform
	//void setRotation(const vec3& rotation) { m_Rotation = rotation; } at the very least quats...
	void setPosition(const float x, const float y, const float z) { m_Position = vec3(x, y, z); }
	void setTargetPosition(const float x, const float y, const float z) { m_TargetPosition = vec3(x, y, z); }
	void setTargetPosition(const vec3& newPosition) { m_TargetPosition = newPosition; }
	void setUpVector(const float x, const float y, const float z) { m_Up = vec3(x, y, z); }

	//vec3& getRotation() { return m_Rotation; }
	vec3& getPosition() { return m_Position; }
	vec3& getTargetPosition() { return m_TargetPosition; }
	vec3& getUpVector() { return m_Up; }

	void setAimSensitivity(const float sensitivity) { m_AimSensitivity = sensitivity; }	//Do I need a get()?
	void setMovementSpeed(const float speed) { m_MovementSpeed = speed; }

	void setProjectionMatrix(const float fov, const int aspectRatio, const float near, const float far) { m_ProjectionMatrix = perspective(radians(fov), float(aspectRatio), near, far); };

	mat4& getViewMatrix() { return m_ViewMatrix; }
	mat4& getProjectionMatrix() { return m_ProjectionMatrix; }

	void moveForward(float value);
	void moveRight(float value);
	void turn(float mouseX, float mouseY);
	void update();


private:
	vec3 m_Position;
	//glm::vec3 m_Rotation;  // Maybe?
	vec3 m_TargetPosition;
	vec3 m_Up;
	vec3 m_DeltaPosition;
	vec3 m_CameraPosition;

	mat4 m_ViewMatrix;
	mat4 m_ProjectionMatrix;

	float m_AimSensitivity;
	float m_MovementSpeed;

	float m_TurnX;
	float m_TurnY;

	float m_CameraBoomLength;
};

