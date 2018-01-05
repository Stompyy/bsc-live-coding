#include "Raycast.h"

// http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-a-physics-library/

Raycast::Raycast()
{
	rayStart_NDC = vec4(
		0.0f, 0.0f,	// centre of the screen [-1, 1]
		-1.0,		// The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);
	rayEnd_NDC = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Raycast::update(Camera* camera, btDynamicsWorld* dynamicsWorld)
{
	// The Projection matrix goes from Camera Space to NDC.
	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(camera->getProjectionMatrix());

	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(camera->getViewMatrix());

	glm::vec4 rayStart_camera = InverseProjectionMatrix * rayStart_NDC;		rayStart_camera /= rayStart_camera.w;
	glm::vec4 rayStart_world = InverseViewMatrix       * rayStart_camera;	rayStart_world /= rayStart_world.w;

	glm::vec4 rayEnd_camera = InverseProjectionMatrix * rayEnd_NDC;			rayEnd_camera /= rayEnd_camera.w;
	glm::vec4 rayEnd_world = InverseViewMatrix       * rayEnd_camera;		rayEnd_world /= rayEnd_world.w;

	// Faster way (just one inverse)
	//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;

	glm::vec3 rayDir_world(rayEnd_world - rayStart_world);
	rayDir_world = glm::normalize(rayDir_world);

	glm::vec3 out_origin = glm::vec3(rayStart_world.x, rayStart_world.y, rayStart_world.z);

	glm::vec3 out_end = out_origin + rayDir_world*1000.0f;

	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(out_origin.x, out_origin.y, out_origin.z),
		btVector3(out_end.x, out_end.y, out_end.z)
	);
	dynamicsWorld->rayTest(
		btVector3(out_origin.x, out_origin.y, out_origin.z),
		btVector3(out_end.x, out_end.y, out_end.z),
		RayCallback
	);

	if (RayCallback.hasHit()) 
	{
		// What to do with this information though?
		printf("mesh: %i \n", (int)RayCallback.m_collisionObject->getUserIndex()); // getUserPointer());
		m_HitRigidBody = const_cast<btRigidBody *>(btRigidBody::upcast(RayCallback.m_collisionObject));

		//btRigidBody hitObjectValue = btRigidBody(*hitRigidBody);
		//hitObjectValue.activate(true);
		//hitObjectValue.applyCentralForce(btVector3(0.0f, 1000.0f, 0.0f));
	}
	else {
		m_HitRigidBody = nullptr;
		printf("background \n");
	}
}

void Raycast::destroy()
{
}

Raycast::~Raycast()
{
	destroy();
}
