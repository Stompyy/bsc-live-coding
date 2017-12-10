#pragma once

#include <glm\glm.hpp>

using namespace glm;

class Material
{
public:
	Material();
	~Material();

	void setAmbientColour(const float r, const float g, const float b, const float a) { m_AmbientColour = vec4(r, g, b, a); }
	void setDiffuseColour(const float r, const float g, const float b, const float a) { m_DiffuseColour = vec4(r, g, b, a); }
	void setSpecularColour(const float r, const float g, const float b, const float a) { m_SpecularColour = vec4(r, g, b, a); }
	void setAmbientColour(const float r, const float g, const float b) { m_AmbientColour = vec4(r, g, b, 1.0f); }
	void setDiffuseColour(const float r, const float g, const float b) { m_DiffuseColour = vec4(r, g, b, 1.0f); }
	void setSpecularColour(const float r, const float g, const float b) { m_SpecularColour = vec4(r, g, b, 1.0f); }
	void setSpecularPower(const float newSpecularPower) { m_SpecularPower = newSpecularPower; }
	vec4& getAmbientColour() { return m_AmbientColour; }
	vec4& getDiffuseColour() { return m_DiffuseColour; }
	vec4& getSpecularColour() { return m_SpecularColour; }
	float getSpecularPower() { return m_SpecularPower; }

private:
	vec4 m_AmbientColour;
	vec4 m_DiffuseColour;
	vec4 m_SpecularColour;
	float m_SpecularPower;
};

