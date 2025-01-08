#version 330 core
out vec4 FragmentColor;

in vec3 inTexcoord;


uniform samplerCube skybox;

void main()
{
    FragmentColor = texture(skybox, inTexcoord);
}