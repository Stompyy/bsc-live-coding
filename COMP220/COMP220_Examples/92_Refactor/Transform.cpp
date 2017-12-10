#include "Transform.h"



Transform::Transform()
{
	m_Position = vec3(0.0f, 0.0f, 0.0f);
	m_Scale = vec3(1.0f, 1.0f, 1.0f);
	m_Rotation = vec3(0.0f, 0.0f, 0.0f);
}


Transform::~Transform()
{
}
