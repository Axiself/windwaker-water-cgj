#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inTangent;
in vec3 inBitangent;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;
out vec4 exColor;
out vec3 exLightPos;
out mat3 exTBN;

uniform mat4 ModelMatrix;
uniform vec4 inColor;
uniform vec3 inLightPos;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void)
{
	exPosition = inPosition;
	exTexcoord = inTexcoord;
	exNormal = inNormal;
	exColor = inColor;
	exLightPos = inLightPos;

	vec3 T = normalize(mat3(ModelMatrix) * inTangent);
	vec3 B = normalize(mat3(ModelMatrix) * inBitangent);
	vec3 N = normalize(mat3(ModelMatrix) * inNormal);
	exTBN = mat3(T, B, N);

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}
