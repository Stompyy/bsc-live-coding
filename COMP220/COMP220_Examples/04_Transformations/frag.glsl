#version 330 core

out vec4 colour;
in vec4 vertexColourOut;

uniform vec4 fragColour=vec4(1.0,1.0,1.0,1.0);

void main()
{
	//colour=fragColour;
	colour = vertexColourOut;
}