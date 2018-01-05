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

//multiple lights - http://www.geeks3d.com/20091013/shader-library-phong-shader-with-multiple-lights-glsl/
uniform vec4 lightOneDirection;
uniform vec4 ambientLightOneColour;
uniform vec4 diffuseLightOneColour;
uniform vec4 specularLightOneColour;

uniform vec4 lightTwoDirection;
uniform vec4 ambientLightTwoColour;
uniform vec4 diffuseLightTwoColour;
uniform vec4 specularLightTwoColour;

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

	ambient = ambientMaterialColour * ambientLightOneColour * ambientLightTwoColour;

	// Light number one
	// Calculate diffuse lighting, normals dot product light direction, intensity of diffuse light
	float nDotlL1 = clamp(dot(worldNormals.xyz, lightOneDirection.xyz), 0, 1);
	diffuse = diffuseMaterialColour * diffuseLightOneColour * nDotlL1;

	// Calculate specular lighting
	vec3 halfwayL1 = normalize(lightOneDirection.xyz + viewDirection);
	float nDothL1 = clamp(dot(worldNormals.xyz, halfwayL1), 0, 1);
	float specularIntensityL1 = pow(nDothL1, specularPower);
	specular = specularMaterialColour * specularLightOneColour * specularIntensityL1;

	// Light number two
	// Calculate diffuse lighting, normals dot product light direction, intensity of diffuse light
	float nDotlL2 = clamp(dot(worldNormals.xyz, lightTwoDirection.xyz), 0, 1);
	diffuse += diffuseMaterialColour * diffuseLightTwoColour * nDotlL2;
	//diffuse = diffuseMaterialColour * diffuseLightOneColour * nDotlL1 * diffuseLightTwoColour * nDotlL2

	// Calculate specular lighting
	vec3 halfwayL2 = normalize(lightTwoDirection.xyz + viewDirection);
	float nDothL2 = clamp(dot(worldNormals.xyz, halfwayL2), 0, 1);
	float specularIntensityL2 = pow(nDothL2, specularPower);
	specular += specularMaterialColour * specularLightTwoColour * specularIntensityL2;
	//specular = specularMaterialColour * specularLightOneColour * specularIntensityL1 * specularLightOneColour * specularIntensityL1;

	gl_Position = MVPMatrix * vec4(vertexPosition,1.0f);

}