#include "Light.h"



Light::Light()
{
	transform = new Transform();
	colour = new Material();
	
	m_Direction = vec3(0.0f, 0.0f, -1.0f);
}


Light::~Light()
{
	destroy();
}

void Light::destroy()
{
	delete colour;
	delete transform;
}
