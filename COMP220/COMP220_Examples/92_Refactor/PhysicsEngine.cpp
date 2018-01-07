#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
	//collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_CollisionConfiguration = new btDefaultCollisionConfiguration();

	//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);

	//btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_OverlappingPairCache = new btDbvtBroadphase();

	//the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_Solver = new btSequentialImpulseConstraintSolver;

	// Contains the physics system for the scene
	m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_OverlappingPairCache, m_Solver, m_CollisionConfiguration);
	m_DynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	// Ensures initial empty value
	m_RigidBodies.clear();
}


PhysicsEngine::~PhysicsEngine()
{
	destroy();
}

void PhysicsEngine::destroy()
{
	for (btRigidBody* rigidBody : m_RigidBodies)
	{
		m_DynamicsWorld->removeRigidBody(rigidBody);
		delete rigidBody->getCollisionShape();
		delete rigidBody->getMotionState();
		delete rigidBody;
	}
	if (m_DynamicsWorld)			{ delete m_DynamicsWorld;			m_DynamicsWorld = nullptr; }
	if (m_Solver)					{ delete m_Solver;					m_Solver = nullptr; }
	if (m_OverlappingPairCache)		{ delete m_OverlappingPairCache;	m_OverlappingPairCache = nullptr; }
	if (m_Dispatcher)				{ delete m_Dispatcher;				m_Dispatcher = nullptr; }
	if (m_CollisionConfiguration)	{ delete m_CollisionConfiguration;	m_CollisionConfiguration = nullptr; }
}
