#version 330 core

out vec4 colour;

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;

uniform sampler2D baseTexture;
uniform vec4 fragColour = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	//colour = vertexColourOut;
	
	colour = texture2D(baseTexture, vertexTextureCoordOut);
	
	//colour = vec4(texture(myTextureSampler, UV).rgb, 1.0f);
}