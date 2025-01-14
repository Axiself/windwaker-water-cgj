#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;
out vec4 exColor;
out vec3 exLightPos;
out vec3 exCameraPos;

uniform mat4 ModelMatrix;
uniform vec4 inColor;
uniform vec3 inLightPos;
uniform samplerCube skybox;
uniform vec3 inCameraPos;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void)
{
	exPosition = inPosition;
	exTexcoord = inTexcoord;
	exNormal = normalize(mat3(ModelMatrix) * inNormal);
	exColor = inColor;
	exLightPos = inLightPos;
	exCameraPos = inCameraPos;

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}
