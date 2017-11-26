#version 330 core

in vec2 textureCoordsOut;

out vec4 colour;

uniform sampler2D texture0;

vec3 roundColour(vec3 colour)
{
	float roundAmount = 80.0f;
	float r = float(int(colour.r * 255.0f/roundAmount) * roundAmount/255.0f);
	float g = float(int(colour.g * 255.0f/roundAmount) * roundAmount/255.0f);
	float b = float(int(colour.b * 255.0f/roundAmount) * roundAmount/255.0f);
	return vec3(r, g, b);
}

void main()
{
	vec4 textureColour = texture(texture0, textureCoordsOut);

	colour = vec4(roundColour(textureColour.rgb), 1.0f);
}