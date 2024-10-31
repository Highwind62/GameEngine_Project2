// Targeting version 4.6 of GLSL. If the compiler does not support 4.5 it will cause an error.
#version 460 core

layout(shared) uniform transformBlock
{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 normalModelMatrix;
};

out vec3 worldPos;
out vec3 worldNorm;
out vec2 texCoord0;
out mat3 TBN;

layout (location = 0) in vec4 vertexPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

void main()
{
	// Normal Mapping
	vec3 T = normalize(vec3(modelMatrix * vec4(aTangent, 0.0)));
	vec3 B = normalize(vec3(modelMatrix * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(modelMatrix * vec4(normal, 0.0)));

	TBN = (mat3(T, B, N));

	// Transform the position of the vertex to clip 
	// coordinates (minus perspective division)
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertexPosition;

	// Transform the position of the vertex to world coords for lighting
	worldPos = (modelMatrix * vertexPosition).xyz;

	// Transform the normal to world coords for lighting
	worldNorm = normalize(mat3(normalModelMatrix) * normal); 

	// Pass through the texture coordinate
	texCoord0 = vertexTexCoord;

}