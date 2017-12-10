#version 330 core

in vec2 textureCoordsOut;

out vec4 colour;

uniform sampler2D texture0;

float getLuminescence(vec3 colour)
{
	return (colour.r + colour.g + colour.b) / 3.0f;
}

void main()
{
	vec4 textureColour = texture(texture0, textureCoordsOut);
	float luminescence = getLuminescence(textureColour.rgb);

	//if (luminescence < 0.2) colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//else colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	colour = vec4(luminescence, luminescence, luminescence, 1.0f);
}