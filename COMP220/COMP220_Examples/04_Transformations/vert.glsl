#version 330 core

layout(location=0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColourIn;

out vec4 vertexColourOut;

uniform float time = 0.0f;

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projectionMatrix = mat4(1.0f);

void main()
{
	vertexColourOut = vec4(vertexColourIn, 1.0f);

	mat4 MVPMatrix = projectionMatrix*viewMatrix,modelMatrix;

	gl_Position = MVPMatrix*vec4(vertexPosition,1.0f);
}