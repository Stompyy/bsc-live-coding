#include "Camera.h"

Camera::Camera()
{
	m_Position = vec3(0.0f, 0.0f, -10.0f);
	//m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Up = vec3(0.0f, 1.0f, 0.0f);
	m_TargetPosition = vec3(0.0f, 0.0f, 0.0f);
	m_DeltaPosition = vec3(0.0f, 0.0f, 0.0f);

	m_CameraBoomLength = 10.0f;
	m_CameraPosition = m_Position - m_CameraBoomLength * normalize(m_TargetPosition - m_Position);

	m_ViewMatrix = lookAt(m_CameraPosition, m_TargetPosition, m_Up);
	m_ProjectionMatrix = perspective(radians(90.0f), float(1000 / 800), 0.1f, 100.0f);

	m_AimSensitivity = 0.001f;
	m_MovementSpeed = 0.2f;

	// Starts off at 90 degrees?
	m_TurnX = -90.0f;
	m_TurnY = 0.0f;
}

Camera::~Camera()
{
}

void Camera::moveForward(float value)
{
	// Move the camera position and the target position towards the target position
	m_DeltaPosition = normalize(m_TargetPosition - m_Position) * m_MovementSpeed * value;
	m_Position += vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z);
	m_TargetPosition += vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z);
}

void Camera::moveRight(float value)
{
	// Move the camera position and the target position at a right angle to the target position
	m_DeltaPosition = cross((normalize(m_TargetPosition - m_Position)), m_Up) * m_MovementSpeed * value;
	m_Position += vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z);
	m_TargetPosition += vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z);
}

void Camera::turn(float mouseX, float mouseY)
{
	// Adjust viewing angle variables based on mouse movement
	m_TurnX += mouseX * m_AimSensitivity;
	m_TurnY += -tan(mouseY * m_AimSensitivity);

	// Clamp Y to avoid gimble lock as tan tends towards infinity
	if (m_TurnY > 0.95f) m_TurnY = 0.95f;
	else if (m_TurnY < -0.95f)	m_TurnY = -0.95f;

	// Move camera lookatpoint to a trigonometry calculated position
	m_TargetPosition = m_Position + vec3(cos(m_TurnX), m_TurnY, sin(m_TurnX));
}

void Camera::update()
{
	// Update the camera position to be at a third person view
	m_CameraPosition = m_Position - m_CameraBoomLength * normalize(m_TargetPosition - m_Position);

	// Update view matrix
	m_ViewMatrix = lookAt(m_CameraPosition, m_TargetPosition, m_Up);
}
