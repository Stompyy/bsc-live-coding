#include "Player.h"

Player::Player()
{
	camera = new Camera();
	//FBXTexture = new EmbeddedTextureFBXLoader();

	//LoadTextureFromFBXFile("archer.FBX");

	m_DeltaPosition = vec3(0.0f);
	m_Up = vec3(0.0f, 1.0f, 0.0f);
	m_WalkingSpeed = 0.2f;
	m_RunningSpeed = 0.6f;
	m_MovementSpeed = 0.2f;
	m_JumpForce = 200.0f;

	// Sets the initial player rotation in line with the camera rotation
	updateRotation();
}

Player::~Player()
{
	destroy();
}

void Player::moveForward()
{
	// Move the camera position and the target position towards the target position
	m_DeltaPosition = normalize(camera->target->getPosition() - camera->worldLocation->getPosition()) * m_MovementSpeed;
	transform->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
	camera->target->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
	camera->worldLocation->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));

	// Update rotation when moved
	updateRotation();
}

void Player::moveRight()
{
	// Move the camera position and the target position at a right angle to the target position
	m_DeltaPosition = cross((normalize(camera->target->getPosition() - camera->worldLocation->getPosition())), m_Up) * m_MovementSpeed;
	transform->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
	camera->target->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));
	camera->worldLocation->addPosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));

	// Update rotation when moved
	updateRotation();
}

void Player::jump()
{
	// Activate the rigidbody and apply a central force to it straight up along the y-axis
	physics->getRigidBody()->activate(true);
	physics->getRigidBody()->applyCentralForce(btVector3(0.0f, m_JumpForce, 0.0f));
}

void Player::updateRotation()
{
	// Updates the player transform's rotation to the camera's rotation. Using getFloorPosition() to use y=0 vec3s
	transform->setRotation(conjugate(toQuat(lookAt(camera->target->getFloorPosition(), transform->getFloorPosition(), m_Up))));

	//transform->updateOrientation(camera->target->getFloorPosition()); // Could not get this to work?
}

void Player::destroy()
{
	delete camera;
}

void Player::updateMovement()
{
	// Adjust the camera Position to the players position
	camera->worldLocation->setPosition(transform->getPosition());

	// Create a temporary bullet vector3 to update the physics position
	btVector3 tempPos = btVector3(
		camera->worldLocation->getPosition().x,
		camera->worldLocation->getPosition().y,
		camera->worldLocation->getPosition().z);

	// Update the physics position
	physics->getRigidBody()->getWorldTransform().setOrigin(tempPos);
	physics->getTransform().setOrigin(tempPos);
}

