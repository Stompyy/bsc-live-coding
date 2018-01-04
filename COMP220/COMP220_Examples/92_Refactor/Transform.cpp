#include "Transform.h"

Transform::Transform()
{
	// Default values
	m_Position = vec3(0.0f, 0.0f, 0.0f);
	m_Scale = vec3(1.0f, 1.0f, 1.0f);

	// identity quaternian has no rotation
	m_Rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
}


Transform::~Transform()
{
}
