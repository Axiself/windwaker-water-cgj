#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exLightPos;
in vec4 exColor;

out vec4 FragmentColor;

vec4 diffuseColor(void) {
    float ambient = 0.3;
    vec3 N = normalize(exNormal);          
    vec3 lightDir = normalize(exLightPos - exPosition); 
    float intensity = max(dot(N, lightDir), ambient);
    return vec4(intensity);                
}

void main(void)
{
    vec4 color;
    color = diffuseColor() * exColor;
    FragmentColor = color;
}