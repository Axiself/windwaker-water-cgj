#version 330 core
in vec3 inPosition;

out vec3 inTexcoord;

uniform mat4 ModelMatrix;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main()
{
    vec4 pos = ProjectionMatrix * mat4(mat3(ViewMatrix)) * vec4(inPosition, 1.0f);

    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);

    inTexcoord = vec3(inPosition.x, inPosition.y, inPosition.z);
}