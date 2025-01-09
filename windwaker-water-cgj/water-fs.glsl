#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exLightPos;
in vec4 exColor;
in vec3 exCameraPos;

out vec4 FragmentColor;
uniform samplerCube skybox;

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
    vec3 N = normalize(exNormal);
    //vec3 L = normalize(exLightPos - exPosition);
    vec3 V = normalize(exCameraPos - exPosition);
    //vec3 H = normalize(L + V);
    //float levels = 4.0;
    //float shininess = 32.0;
    //vec3 ambient = vec3(0.5, 0.5, 0.5);

    //float diffuse = max(dot(N, L), 0.0);
    //float quantized = floor(levels * diffuse) / levels;

    //vec3 reflectDir = reflect(-L, N);
    //float spec = pow(max(dot(N, H), 0.0), shininess);
    //float quantizedSpec = floor(levels * spec) / levels;
    //vec3 specular = vec3(1.0, 1.0, 1.0) * quantizedSpec;

    //vec3 lighting = ambient + quantized + specular;
    
    vec3 reflectionDir = normalize(reflect(-V, N));
    vec3 refractionDir = normalize(refract(-V, N, 1.0 / 1.33));

    vec3 reflectionColor = texture(skybox, reflectionDir).rgb;
    vec3 refractionColor = texture(skybox, refractionDir).rgb;

    float fresnel = 0.1 + 0.9 * pow(1.0 - max(dot(N, -V), 0.0), 3.0);

    vec3 color = mix(reflectionColor, refractionColor, fresnel);

    //FragmentColor = vec4(color * exColor.rgb, 1.0);
    FragmentColor = vec4(texture(skybox, reflectionDir).rgb, 1.0); // Only reflection
}
