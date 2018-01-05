#version 330 core

out vec4 colour;

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;
in vec4 diffuse;
in vec4 specular;
in vec4 ambient;

uniform sampler2D baseTexture;
uniform vec4 fragColour; 

void main()
{
	colour = texture(baseTexture, vertexTextureCoordOut) * diffuse + specular + ambient*texture(baseTexture, vertexTextureCoordOut);

	//colour = texture(baseTexture, vertexTextureCoordOut) * diffuse*0.8f + specular + ambient/10.0f;

	//colour = vec4(0.1f, 0.1f, 0.1f, 1.0f) + ambient + specular;
}