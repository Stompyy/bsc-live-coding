#include "Player.h"



Player::Player()
{
	m_Camera = new Camera();
	m_MovementSpeed = 0.2f;
}


Player::~Player()
{
}

void Player::moveForward(float value)
{
	m_DeltaPosition = normalize(m_Camera->getTargetPosition() - transform->getPosition()) * m_MovementSpeed * value;
	transform->getPosition() += vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z);
	m_Camera->setTargetPosition(m_Camera->getTargetPosition() + vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
}

void Player::moveRight(float value)
{
	m_DeltaPosition = cross((normalize(m_Camera->getTargetPosition() - transform->getPosition())), m_Camera->getUpVector()) * m_MovementSpeed * value;
	transform->getPosition() += vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z);
	m_Camera->setTargetPosition(m_Camera->getTargetPosition() + vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
}

