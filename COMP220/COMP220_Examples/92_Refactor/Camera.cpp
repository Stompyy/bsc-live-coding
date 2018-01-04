#include "Camera.h"

Camera::Camera()
{
	worldLocation = new Transform();
	target = new Transform();

	m_Up = vec3(0.0f, 1.0f, 0.0f);

	m_CameraBoomLength = 4.0f;
	m_CameraPivotHeight = 2.5f;

	// keep at a good distance to stop jarring jumping view changes
	m_CameraTargetDistance = 50.0f;

	m_CameraLowerLimitY = -5.0f;
	m_CameraUpperLimitY = 0.8f;

	m_ProjectionMatrix = perspective(radians(90.0f), float(1000 / 800), 0.1f, 100.0f);

	m_AimSensitivity = 0.001f;

	m_TurnX = 0.0f;
	m_TurnY = 0.0f;

	// Set up initial view
	turn(0.0f, 0.0f);
	update();
}

Camera::~Camera()
{
	destroy();
}

void Camera::turn(float mouseX, float mouseY)
{
	// Adjust viewing angle variables based on mouse movement
	m_TurnX += mouseX * m_AimSensitivity;
	m_TurnY += -tan(mouseY * m_AimSensitivity);

	// Clamp Y to avoid going out of scope as tan() tends towards infinity, and to avoid floor clipping
	if (m_TurnY > m_CameraUpperLimitY)
		m_TurnY = m_CameraUpperLimitY;
	else if (m_TurnY < m_CameraLowerLimitY)
		m_TurnY = m_CameraLowerLimitY;

	// Move camera lookatpoint to a trigonometry calculated position
	target->setPosition(worldLocation->getPosition() + vec3(cos(m_TurnX), m_TurnY, sin(m_TurnX)) * m_CameraTargetDistance);
}

void Camera::update()
{
	// Set the position at which the camera boom pivots around.
	m_CameraPivotPosition = vec3(
		worldLocation->getPosition().x,
		worldLocation->getPosition().y + m_CameraPivotHeight,
		worldLocation->getPosition().z
	);

	// Update the camera position to be at a third person view
	m_CameraPosition = m_CameraPivotPosition - m_CameraBoomLength * normalize(target->getPosition() - m_CameraPivotPosition);

	// Update view matrix
	m_ViewMatrix = lookAt(m_CameraPosition, target->getPosition(), m_Up);
}

void Camera::destroy()
{
	delete target;
	delete worldLocation;
}
