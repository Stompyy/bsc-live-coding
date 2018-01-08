#include "Physics.h"

Physics::Physics()
{
	m_CollisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	m_Transform.setIdentity();
	m_Transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
	m_Mass = 1.0f;
	m_Inertia = btVector3(0.0f, 0.0f, 0.0f);
	m_CollisionShape->calculateLocalInertia(m_Mass, m_Inertia);
	m_MotionState = new btDefaultMotionState(m_Transform);
	btRigidBody::btRigidBodyConstructionInfo m_RigidBodyInfo(m_Mass, m_MotionState, m_CollisionShape, m_Inertia);
	m_RigidBody = new btRigidBody(m_RigidBodyInfo);

	m_IsPhysicsActive = false;
}


Physics::~Physics()
{
	destroy();
}

void Physics::createRigidBody()
{
	m_MotionState = new btDefaultMotionState(m_Transform);
	btRigidBody::btRigidBodyConstructionInfo m_RigidBodyInfo(m_Mass, m_MotionState, m_CollisionShape, m_Inertia);
	m_RigidBody = new btRigidBody(m_RigidBodyInfo);
}

void Physics::update()
{
	// Check if Physics is active and activate if true
	m_RigidBody->activate(m_IsPhysicsActive);

	// Update physics transform to the rigidbody transform
	m_Transform = m_RigidBody->getWorldTransform();
}

void Physics::destroy()
{
	if (m_RigidBody)		{ delete m_RigidBody;		m_RigidBody = nullptr; }
	if (m_MotionState)		{ delete m_MotionState;		m_MotionState = nullptr; }
	if (m_CollisionShape)	{ delete m_CollisionShape;	m_CollisionShape = nullptr; }
}
