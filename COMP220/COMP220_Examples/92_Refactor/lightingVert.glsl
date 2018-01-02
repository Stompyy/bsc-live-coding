#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColourIn;
layout(location = 2) in vec2 vertexTextureCoord;
layout(location = 3) in vec3 vertexNormals;

// MVP
uniform mat4 modelMatrix; 
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Camera position
uniform vec4 cameraPosition;

// Lighting
uniform vec4 lightDirection;
uniform vec4 ambientLightColour;
uniform vec4 diffuseLightColour;
uniform vec4 specularLightColour;

// Material
uniform vec4 ambientMaterialColour;
uniform vec4 diffuseMaterialColour;
uniform vec4 specularMaterialColour;
uniform float specularPower;   // Roughness component of material/how much light it reflects


out vec4 vertexColourOut;
out vec2 vertexTextureCoordOut;
out vec4 diffuse;
out vec4 specular;
out vec4 ambient;

void main()
{
	vertexColourOut = vertexColourIn;
	vertexTextureCoordOut = vertexTextureCoord;

	// Model View Projection Matrix 
	mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;

	vec4 worldNormals = normalize(modelMatrix * vec4(vertexNormals, 1.0f));

	// World position of vertex
	vec4 worldPosition = modelMatrix * vec4(vertexPosition, 1.0f);

	vec3 viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);

	ambient = ambientMaterialColour * ambientLightColour;

	// Calculate diffuse lighting, normals dot product light direction, intensity of diffuse light
	float nDotl = clamp(dot(worldNormals.xyz, lightDirection.xyz), 0, 1);
	diffuse = diffuseMaterialColour * diffuseLightColour * nDotl;

	// Calculate specular lighting
	vec3 halfway = normalize(lightDirection.xyz + viewDirection);
	float nDoth = clamp(dot(worldNormals.xyz, halfway), 0, 1);
	float specularIntensity = pow(nDoth, specularPower);
	specular = specularMaterialColour * specularLightColour * specularIntensity;

	gl_Position = MVPMatrix * vec4(vertexPosition,1.0f);

}