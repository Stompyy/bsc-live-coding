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
}

void Physics::updateMotionState()
{
	m_MotionState = new btDefaultMotionState(m_Transform);
	btRigidBody::btRigidBodyConstructionInfo m_RigidBodyInfo(m_Mass, m_MotionState, m_CollisionShape, m_Inertia);
	m_RigidBody = new btRigidBody(m_RigidBodyInfo);
}
