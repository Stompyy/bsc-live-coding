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

void Physics::destroy()
{
	if (m_RigidBody)		{ delete m_RigidBody;		m_RigidBody = nullptr; }
	if (m_MotionState)		{ delete m_MotionState;		m_MotionState = nullptr; }
	if (m_CollisionShape)	{ delete m_CollisionShape;	m_CollisionShape = nullptr; }
}
