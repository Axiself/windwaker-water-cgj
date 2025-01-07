#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exLightPos;
in vec4 exColor;

out vec4 FragmentColor;

vec3 constantColor(void) {
    return vec3(0.5);
}

vec3 positionColor(void) {
    return (exPosition + vec3(1.0)) * 0.5;
}

vec3 uvColor(void) {
    return vec3(exTexcoord, 0.0);
}

vec3 normalColor(void) {
    return (exNormal + vec3(1.0));
}

vec4 diffuseColor(void) {
    float ambient = 0.2;
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
