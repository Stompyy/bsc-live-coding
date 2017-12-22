#version 330 core

in vec2 textureCoordsOut;

out vec4 colour;

uniform sampler2D texture0;

float roundColour(float x)
{
	float roundAmount = 80.0f;
	return clamp(float(int((x * 255.0f/roundAmount))) * roundAmount/255.0f, 0, 1);
	//return clamp(float(int((x * 255.0f/roundAmount) + roundAmount/2.0f)) * roundAmount/255.0f, 0, 1);
	//return clamp(float(int((x * 255.0f/roundAmount) + roundAmount/2.0f) + roundAmount/2.0f) * roundAmount/255.0f, 0, 1);
}

vec3 roundVec3Colour(vec3 colour)
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

	//colour = vec4(roundColour(textureColour.r), roundColour(textureColour.g), roundColour(textureColour.b), 1.0f);

	colour = vec4(roundVec3Colour(textureColour.rgb), 1.0f);
	//colour = textureColour;
}