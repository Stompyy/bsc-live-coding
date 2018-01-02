#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "Transform.h"

// Only using glm vector library
using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();

	Transform* worldLocation;
	Transform* target;
	
	void setAimSensitivity(const float sensitivity) { m_AimSensitivity = sensitivity; }

	void setProjectionMatrix(const float fov, const int aspectRatio, const float near, const float far) { m_ProjectionMatrix = perspective(radians(fov), float(aspectRatio), near, far); };

	mat4& getProjectionMatrix() { return m_ProjectionMatrix; }
	mat4& getViewMatrix() { return m_ViewMatrix; }

	void turn(float mouseX, float mouseY);
	void update();

private:
	vec3 m_Up;
	vec3 m_CameraPosition;
	vec3 m_CameraPivotPosition;

	mat4 m_ViewMatrix;
	mat4 m_ProjectionMatrix;

	float m_AimSensitivity;

	float m_TurnX;
	float m_TurnY;

	float m_CameraBoomLength;
	float m_CameraPivotHeight;
	float m_CameraTargetDistance;

	float m_CameraLowerLimitY;
	float m_CameraUpperLimitY;
};

