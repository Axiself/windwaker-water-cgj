#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exLightPos;
in vec4 exColor;
in vec3 exCameraPos;

out vec4 FragmentColor;
uniform samplerCube skybox;

void main(void)
{
    vec3 N = normalize(exNormal);
    vec3 V = normalize(exCameraPos - exPosition);

    vec3 reflectionDir = normalize(reflect(-V, N));
    vec3 refractionDir = normalize(refract(-V, N, 1.0 / 1.33));

    vec3 reflectionColor = texture(skybox, reflectionDir).rgb;
    vec3 refractionColor = texture(skybox, refractionDir).rgb;

    float fresnel = 0.3 + 0.7 * pow(1.0 - max(dot(N, -V), 0.0), 3.0);

    vec3 color = mix(reflectionColor, refractionColor, fresnel);

    FragmentColor = vec4(color * exColor.rgb, 1.0);
}
