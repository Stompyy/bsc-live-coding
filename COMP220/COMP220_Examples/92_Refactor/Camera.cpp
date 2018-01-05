#include "Camera.h"

Camera::Camera()
{
	m_WorldLocation = new Transform();
	m_Target = new Transform();

	m_Up = vec3(0.0f, 1.0f, 0.0f);

	m_CameraBoomLength = 4.0f;
	m_CameraPivotHeight = 2.5f;
	m_ZoomSpeed = 0.1f;
	m_MinimumDistanceToPlayer = 0.5f;
	m_MaximumDistanceToPlayer = 10.f;

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
	m_Target->setPosition(m_WorldLocation->getPosition() + vec3(cos(m_TurnX), m_TurnY, sin(m_TurnX)) * m_CameraTargetDistance);
}

void Camera::movePosition(const vec3 & deltaPosition)
{
	m_WorldLocation->movePosition(vec3(deltaPosition.x, 0.0f, deltaPosition.z));
	m_Target->movePosition(vec3(deltaPosition.x, 0.0f, deltaPosition.z));
}

// If Camera is within the distance to player tolerances, then move
void Camera::moveCloser()
{
	if (m_CameraBoomLength > m_MinimumDistanceToPlayer)
		m_CameraBoomLength -= m_ZoomSpeed;
}

void Camera::moveAway()
{
	if (m_CameraBoomLength < m_MaximumDistanceToPlayer)
		m_CameraBoomLength += m_ZoomSpeed;
}

void Camera::update()
{
	// Set the position at which the camera boom pivots around.
	m_CameraPivotPosition = vec3(
		m_WorldLocation->getPosition().x,
		m_WorldLocation->getPosition().y + m_CameraPivotHeight,
		m_WorldLocation->getPosition().z
	);

	// Update the camera position to be at a third person view
	m_CameraPosition = m_CameraPivotPosition - m_CameraBoomLength * normalize(m_Target->getPosition() - m_CameraPivotPosition);

	// Update view matrix
	m_ViewMatrix = lookAt(m_CameraPosition, m_Target->getPosition(), m_Up);
}

void Camera::destroy()
{
	delete m_Target;
	delete m_WorldLocation;
}
