#pragma once

#include "btBulletDynamicsCommon.h"
#include <vector>

// Bullet real-time physics simulation
// https://github.com/bulletphysics/bullet3
// https://pybullet.org/wordpress/

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	btDiscreteDynamicsWorld* getDynamicsWorld() { return m_DynamicsWorld; }

	// Adds a new rigidBody to the dynamics world, and reference vector
	void addRigidBody(btRigidBody* rigidBody) { m_DynamicsWorld->addRigidBody(rigidBody); m_RigidBodies.push_back(rigidBody); }

	std::vector<btRigidBody*> getAllRigidBodies() { return m_RigidBodies; }

	void destroy();

private:
	btDefaultCollisionConfiguration* m_CollisionConfiguration;
	btCollisionDispatcher* m_Dispatcher;
	btBroadphaseInterface* m_OverlappingPairCache;
	btSequentialImpulseConstraintSolver* m_Solver;
	btDiscreteDynamicsWorld* m_DynamicsWorld;

	// Reference vector of all current rigidBodies for quick access, and used in the destroy function
	std::vector<btRigidBody*>m_RigidBodies;
};

