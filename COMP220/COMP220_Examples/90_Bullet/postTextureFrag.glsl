#version 330 core

in vec2 textureCoordsOut;

out vec4 colour;

uniform sampler2D texture0;

void main()
{
	colour = texture(texture0, textureCoordsOut);
}