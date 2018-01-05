#pragma once

#include <btBulletDynamicsCommon.h>

// Bullet real-time physics simulation
// https://github.com/bulletphysics/bullet3
// https://pybullet.org/wordpress/


/*
vec3 btQuatToGlmVec3(const btQuaternion& q)
// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
{
double roll, pitch, yaw;

// roll (x-axis rotation)
double sinr = +2.0 * (q.getW() * q.getX() + q.getY() * q.getZ());
double cosr = +1.0 - 2.0 * (q.getX() * q.getX() + q.getY() * q.getY());
roll = atan2(sinr, cosr);

// pitch (y-axis rotation)
double sinp = +2.0 * (q.getW() * q.getY() - q.getZ() * q.getX());
if (fabs(sinp) >= 1)
pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
else
pitch = asin(sinp);

// yaw (z-axis rotation)
double siny = +2.0 * (q.getW() * q.getZ() + q.getX() * q.getY());
double cosy = +1.0 - 2.0 * (q.getY() * q.getY() + q.getZ() * q.getZ());
yaw = atan2(siny, cosy);

return vec3(roll, pitch, yaw);
}
*/


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
	void setPosition(const btScalar x, const btScalar y, const btScalar z) { m_Transform.setOrigin(btVector3(x, y, z)); }

	btTransform& getTransform() { return m_Transform; }
	btRigidBody* getRigidBody() { return m_RigidBody; }
	btVector3& getInertia() { return m_Inertia; }
	btDefaultMotionState* getMotionState() { return m_MotionState; }

	void createRigidBody();
	void destroy();

private:
	btCollisionShape* m_CollisionShape;
	btTransform m_Transform;
	btScalar m_Mass;
	btVector3 m_Inertia;
	btDefaultMotionState* m_MotionState;
	btRigidBody* m_RigidBody;
};


