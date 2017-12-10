#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColourIn;
layout(location = 2) in vec2 vertexTextureCoord;

out vec4 vertexColourOut;
out vec2 vertexTextureCoordOut;

uniform mat4 modelMatrix; // = mat4(1.0f);
uniform mat4 viewMatrix; // = mat4(1.0f);
uniform mat4 projectionMatrix; // = mat4(1.0f);

void main()
{
	//vertexColourOut = vec4(vertexColourIn, 1.0f);
	vertexColourOut = vertexColourIn;
	vertexTextureCoordOut = vertexTextureCoord;

	// Model View Projection Matrix - Why comma though?
	mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;  //viewMatrix,modelMatrix;

	gl_Position = MVPMatrix * vec4(vertexPosition,1.0f);
}