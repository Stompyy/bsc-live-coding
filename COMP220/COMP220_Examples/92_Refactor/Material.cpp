#include "Material.h"

Material::Material()
{
	m_AmbientColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DiffuseColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularPower = 25.0f;
}


Material::~Material()
{
}
