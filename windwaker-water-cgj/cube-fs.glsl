#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exColor;

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

vec3 diffuseColor(void) {
    float ambient = 0.2;
    vec3 N = normalize(exNormal);          
    vec3 lightDir = normalize(vec3(0.5, 1, 0.5)); 
    float intensity = max(dot(lightDir, N), ambient);
    return vec3(intensity + ambient);                
}

void main(void)
{
    vec3 color;
    // color = constantColor();
    // color = positionColor();
    // color = uvColor();
    color = diffuseColor() * exColor;
    FragmentColor = vec4(color, 1.0);
}
