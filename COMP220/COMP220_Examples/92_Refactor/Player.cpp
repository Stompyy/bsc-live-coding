#include "Player.h"

Player::Player()
{
	m_Camera = new Camera();

	m_DeltaPosition = vec3(0.0f);
	m_WalkingSpeed = 0.2f;
	m_RunningSpeed = 0.6f;
	m_MovementSpeed = 0.2f;
	m_JumpForce = 200.0f;
	m_JumpForwardForceMultiplier = 0.1f;

	// Sets the initial player rotation in line with the camera rotation
	updateRotation();
}

Player::~Player()
{
	destroy();
}

// Move towards the camera's target position
void Player::moveForward(const float direction)
{
	// Calculate the delta position
	m_DeltaPosition = normalize(m_Camera->getTarget()->getPosition() - m_Camera->getWorldLocation()->getPosition()) * m_MovementSpeed * direction;

	// Move the gameObject's transform horizontally by the delta position
	getTransform()->movePosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));

	// Update camera position
	m_Camera->movePosition(m_DeltaPosition);

	// Update rotation when moved
	updateRotation();
}

// Move perpendicular to the camera's target position
void Player::moveRight(const float direction)
{
	// Calculate the delta position
	m_DeltaPosition = cross((normalize(m_Camera->getTarget()->getPosition() - m_Camera->getWorldLocation()->getPosition())), getTransform()->getUp()) * m_MovementSpeed * direction;

	// Move the gameObject's transform horizontally by the delta position
	getTransform()->movePosition(vec3(m_DeltaPosition.x, 0.0f, m_DeltaPosition.z));

	// Update camera position
	m_Camera->movePosition(m_DeltaPosition);

	// Update rotation when moved
	updateRotation();
}

void Player::jumpUp()
{
	// Activate the rigidbody and apply a central force to it straight up along the y-axis
	getPhysics()->getRigidBody()->activate(true);
	getPhysics()->getRigidBody()->applyCentralForce(btVector3(0.0f, m_JumpForce, 0.0f));

}

void Player::jumpForward()
{
	// Get jump direction (camera direction in this case)
	glm::vec3 jumpDirection = normalize(m_Camera->getTarget()->getPivotPosition() - m_Camera->getWorldLocation()->getPivotPosition()) * m_JumpForwardForceMultiplier;

	// Activate the rigidbody and apply a central force to it up (y=1)and in the jumpDirection
	getPhysics()->getRigidBody()->activate(true);
	getPhysics()->getRigidBody()->applyCentralForce(btVector3(jumpDirection.x, 1.0f, jumpDirection.z) * m_JumpForce);

}

void Player::updateRotation()
{
	// Updates the player transform's rotation to the camera's rotation. Using getFloorPosition() to use y=0 vec3s
	getTransform()->setRotation(conjugate(toQuat(lookAt(m_Camera->getTarget()->getPivotPosition(), getTransform()->getPivotPosition(), getTransform()->getUp()))));
}

void Player::destroy()
{
	delete m_Camera;
}

void Player::updateMovement()
{
	// Adjust the camera Position to the players position
	m_Camera->getWorldLocation()->setPosition(getTransform()->getPosition());

	// Create a temporary bullet vector3 to update the physics position
	btVector3 tempPos = btVector3(
		m_Camera->getWorldLocation()->getPosition().x,
		m_Camera->getWorldLocation()->getPosition().y,
		m_Camera->getWorldLocation()->getPosition().z);

	// Update the physics position
	getPhysics()->getRigidBody()->getWorldTransform().setOrigin(tempPos);
	getPhysics()->getTransform().setOrigin(tempPos);

	// Temporary bullet quaternion to update the physics rotation
	btQuaternion tempRot = btQuaternion(
		getTransform()->getRotation().w,
		getTransform()->getRotation().x,
		getTransform()->getRotation().y,
		getTransform()->getRotation().z
	);

	// Update the physics rotation
	getPhysics()->getRigidBody()->getWorldTransform().setRotation(tempRot);
}

