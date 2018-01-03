#include "Player.h"

Player::Player()
{
	camera = new Camera();
	//FBXTexture = new EmbeddedTextureFBXLoader();

	//LoadTextureFromFBXFile("archer.FBX");

	m_MovementSpeed = 0.2f;
	m_JumpForce = 200.0f;
	m_Up = vec3(0.0f, 1.0f, 0.0f);
}

Player::~Player()
{
}

void Player::moveForward(float value)
{
	// Move the camera position and the target position towards the target position
	m_DeltaPosition = normalize(camera->target->getPosition() - camera->worldLocation->getPosition()) * m_MovementSpeed * value;
	transform->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
	camera->target->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
	camera->worldLocation->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));

	transform->setRotation(conjugate(toQuat(lookAt(camera->target->getFloorPosition(), transform->getFloorPosition(), m_Up))));
	//transform->updateOrientation(camera->target->getFloorPosition()); 
}

void Player::moveRight(float value)
{
	// Move the camera position and the target position at a right angle to the target position
	m_DeltaPosition = cross((normalize(camera->target->getPosition() - camera->worldLocation->getPosition())), m_Up) * m_MovementSpeed * value;
	transform->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
	camera->target->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
	camera->worldLocation->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
}

void Player::jump()
{
	physics->getRigidBody()->activate(true);
	physics->getRigidBody()->applyCentralForce(btVector3(0.0f, m_JumpForce, 0.0f));
}

void Player::updateMovement()
{
	camera->worldLocation->setPosition(transform->getPosition());

	btVector3 tempPos = btVector3(
		camera->worldLocation->getPosition().x,
		camera->worldLocation->getPosition().y,
		camera->worldLocation->getPosition().z);

	physics->getRigidBody()->getWorldTransform().setOrigin(tempPos);
	physics->getTransform().setOrigin(tempPos);
}


