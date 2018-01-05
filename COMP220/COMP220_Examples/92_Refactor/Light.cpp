#include "Light.h"



Light::Light()
{
	m_Transform = new Transform();
	m_Colour = new Material();
	
	m_Direction = vec3(0.0f, 0.0f, -1.0f);
}


Light::~Light()
{
	destroy();
}

void Light::destroy()
{
	delete m_Colour;
	delete m_Transform;
}
