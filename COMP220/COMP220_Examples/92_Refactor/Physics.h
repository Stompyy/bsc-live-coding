#pragma once

#include <btBulletDynamicsCommon.h>

// Bullet real-time physics simulation
// https://github.com/bulletphysics/bullet3
// https://pybullet.org/wordpress/

// Handles the collision and physics motion in the physics engine
class Physics
{
public:
	Physics();
	~Physics();

	void setTransform(const btTransform& newTransform) { m_Transform = newTransform; }
	void setMass(const btScalar newMass) { m_Mass = newMass; }
	void setCollisionShapeSize(const btVector3& collisionSize) { m_CollisionShape = new btBoxShape(collisionSize); }
	void setCollisionShapeSize(const btScalar x, const btScalar y, const btScalar z) { m_CollisionShape = new btBoxShape(btVector3(x, y, z)); }
	void setInertia(const btScalar x, const btScalar y, const btScalar z) { m_Inertia = btVector3(x, y, z); }

	btTransform& getTransform() { return m_Transform; }
	btRigidBody* getRigidBody() { return m_RigidBody; }
	btCollisionShape& getCollisionShape() { return *m_CollisionShape; }
	btScalar getMass() { return m_Mass; }
	btVector3& getInertia() { return m_Inertia; }
	btDefaultMotionState* getMotionState() { return m_MotionState; }

	void updateMotionState();
	void destroy();

private:
	btCollisionShape* m_CollisionShape;
	btTransform m_Transform;
	btScalar m_Mass;
	btVector3 m_Inertia;
	btDefaultMotionState* m_MotionState;
	btRigidBody* m_RigidBody;
};


