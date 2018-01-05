#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "Transform.h"

// Only using glm vector library here
using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();

	// Returns the object's Transform member
	Transform* getWorldLocation() { return m_WorldLocation; }

	// Returns the target's Transform member
	Transform* getTarget() { return m_Target; }

	// Essential in any game engine!
	void setAimSensitivity(const float sensitivity) { m_AimSensitivity = sensitivity; }

	// Sets up the camera scene variables
	void setProjectionMatrix(const float fov, const int aspectRatio, const float near, const float far) { m_ProjectionMatrix = perspective(radians(fov), float(aspectRatio), near, far); };

	mat4& getProjectionMatrix() { return m_ProjectionMatrix; }
	mat4& getViewMatrix() { return m_ViewMatrix; }

	// Used to zoom in/out on player maintaining third person controls
	float getBoomLength() { return m_CameraBoomLength; }
	void setBoomLength(const float newLength) { m_CameraBoomLength = newLength; }

	void turn(float mouseX, float mouseY);
	void movePosition(const vec3& deltaPosition); 
	void moveCloser();
	void moveAway();

	// Updates the view matrix before sending values to the shader
	void update();

	void destroy();

private:
	// The camera has a location and a look at point
	Transform* m_WorldLocation;
	Transform* m_Target;

	vec3 m_Up;
	vec3 m_CameraPosition;

	mat4 m_ViewMatrix;
	mat4 m_ProjectionMatrix;

	// Turning speed
	float m_AimSensitivity;

	// For turning the camera with pitch or yaw
	float m_TurnX;
	float m_TurnY;

	// Third person camera requirements
	float m_CameraBoomLength;
	float m_CameraPivotHeight;
	vec3 m_CameraPivotPosition;

	// when jumping, this point acts as a pivot point for the view
	float m_CameraTargetDistance;

	// To avoid the camera clipping through the ground
	float m_CameraLowerLimitY;
	// To avoid going out of scope as tan() tends towards infinity
	float m_CameraUpperLimitY;

	// Camera moving in and out requirements
	float m_ZoomSpeed;
	float m_MinimumDistanceToPlayer;
	float m_MaximumDistanceToPlayer;
};

