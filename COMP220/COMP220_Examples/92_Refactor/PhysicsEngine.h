#pragma once

#include "btBulletDynamicsCommon.h"
#include <vector>

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	btDiscreteDynamicsWorld* getDynamicsWorld() { return m_DynamicsWorld; }
	void addRigidBody(btRigidBody* rigidBody) { m_DynamicsWorld->addRigidBody(rigidBody); m_RigidBodies.push_back(rigidBody); }

	std::vector<btRigidBody*> getAllRigidBodies() { return m_RigidBodies; }

private:
	btDefaultCollisionConfiguration* m_CollisionConfiguration;
	btCollisionDispatcher* m_Dispatcher;
	btBroadphaseInterface* m_OverlappingPairCache;
	btSequentialImpulseConstraintSolver* m_Solver;
	btDiscreteDynamicsWorld* m_DynamicsWorld;
	std::vector<btRigidBody*>m_RigidBodies;
};

