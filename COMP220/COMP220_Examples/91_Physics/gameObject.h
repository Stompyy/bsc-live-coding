#pragma once

#include "main.h"

/*
* Gameobject class
* rigidbody, mesh, position, rotation, scale
* void Update() {Checks if has valid rigidbody, applys physics tick then TRS to update position for draw()}
* 
* mesh tex vec3s shader, rigidbody, collision shape, set and gets() update() render()
*
*/

class gameObject
{
public:
	gameObject();
	~gameObject();

	void Update();

	vec3 worldLocation;
	vec3 worldRotation;
	vec3 worldScale;

	Mesh* mesh;
	GLint textureLocation;
	btRigidBody* rigidbody;

};

