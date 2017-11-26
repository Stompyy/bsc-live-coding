#version 330 core

layout(location=0) in vec3 vertexPosition_modelspace;

uniform float time=0.0f;

void main()
{
	vec3 newVertexPosition=vertexPosition_modelspace;

	newVertexPosition.y+=sin(time);
	newVertexPosition.x+=cos(time);

	gl_Position=vec4(newVertexPosition,1.0);
}