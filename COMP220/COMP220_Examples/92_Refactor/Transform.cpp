#include "Transform.h"

// Common initialiser values across all different constructors
void Transform::initSecondaryVariables()
{
	m_TranslationMatrix = mat4(1.0f);
	m_ScaleMatrix = mat4(1.0f);
	m_RotationMatrix = mat4(1.0f);
	m_ModelMatrix = mat4(1.0f);

	m_Up = vec3(0.0f, 1.0f, 0.0f);

	m_RotationDemoY = 0.0f;
	m_RotationDemoSpeed = 0.02f;
}

Transform::Transform()
{
	// Default values
	m_Position = vec3(0.0f, 0.0f, 0.0f);
	m_Scale = vec3(1.0f, 1.0f, 1.0f);

	// identity quaternian has no rotation
	m_Rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);

	initSecondaryVariables();
}

Transform::Transform(Transform& transform)
{
	m_Position = transform.getPosition();
	m_Rotation = transform.getRotation();
	m_Scale = transform.getScale();
	initSecondaryVariables();
}

Transform::Transform(const vec3& position, const quat& rotation, const float scale)
{
	m_Position = position;
	m_Rotation = rotation;
	m_Scale = vec3(scale, scale, scale);
	initSecondaryVariables();
}

Transform::Transform(const vec3& position, const quat& rotation, const vec3& scale)
{
	m_Position = position;
	m_Rotation = rotation;
	m_Scale = scale;
	initSecondaryVariables();
}

// Construct transform with euler angle rotation argument for readability
Transform::Transform(const vec3& position, const vec3& rotation, const float scale)
{
	m_Position = position;
	m_Rotation = quat(radians(rotation));
	m_Scale = vec3(scale, scale, scale);
	initSecondaryVariables();
}

Transform::~Transform()
{
}

void Transform::update()
{
	// Update the model matrix
	m_TranslationMatrix = translate(m_Position);
	m_ScaleMatrix = scale(m_Scale);
	m_RotationMatrix = toMat4(m_Rotation);

	// Remember TRS order
	m_ModelMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
}
