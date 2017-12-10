#pragma once

#include <btBulletDynamicsCommon.h>

class Physics
{
public:
	Physics();
	~Physics();

	void setTransform(const btTransform& newTransform) { m_Transform = newTransform; }
	void setMass(const btScalar newMass) { m_Mass = newMass; }
	void setCollisionShapeSize(const btScalar x, const btScalar y, const btScalar z) { m_CollisionShape = new btBoxShape(btVector3(x, y, z)); }
	void setInertia(const btScalar x, const btScalar y, const btScalar z) { m_Inertia = btVector3(x, y, z); }

	btTransform& getTransform() { return m_Transform; }
	btRigidBody* getRigidBody() { return m_RigidBody; }
	btCollisionShape* getCollisionShape() { return m_CollisionShape; }
	btScalar getMass() { return m_Mass; }
	btVector3& getInertia() { return m_Inertia; }

	void updateMotionState();

private:
	btCollisionShape* m_CollisionShape;
	btTransform m_Transform;
	btScalar m_Mass;
	btVector3 m_Inertia;
	btDefaultMotionState* m_MotionState;
	btRigidBody* m_RigidBody;
};

