#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exLightPos;
in vec4 exColor;
in mat3 exTBN;

out vec4 FragmentColor;

uniform sampler2D NormalMap;
uniform sampler2D Texture;

vec4 diffuseColor(void) {
    float ambient = 0.2;
    vec3 N = normalize(exNormal);          
    vec3 lightDir = normalize(exLightPos - exPosition); 
    float intensity = max(dot(N, lightDir), ambient);
    return vec4(intensity);                
}

vec4 normalMapping(void) {
    float ambient = 0.3;
    vec3 normal = texture(NormalMap, exTexcoord).xyz;
    normal = normalize(normal * 2.0 - 1.0);
    normal = -normalize(exTBN * normal);
    vec3 lightDir = normalize(exLightPos - exPosition); 
    float intensity = max(dot(normal, lightDir), ambient);
    return vec4(texture(Texture, exTexcoord).rgb * intensity, 1.0);
}

void main(void)
{
    vec4 color;
    color = normalMapping();
    FragmentColor = color;
}
