// A phong shader
#version 410

struct PointLight {
    vec3 Position;

    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4

in vec3 vNormal;    // Surface normal from mesh
in vec4 vPosition;          // World-space surface position from mesh

out vec4 FragColor;

uniform vec3 CameraPosition; // World-space position of the camera

uniform PointLight pointLights[NR_POINT_LIGHTS];

// MTL Data
uniform vec3 Ka; // Ambient material colour
uniform vec3 Kd; // Diffuse material colour
uniform vec3 Ks; // Specular material colour
uniform float specularPower; // The material specular power

// Light Data
uniform vec3 AmbientColor;      // Ambient light colour
uniform vec3 LightColor;        // Diffuse light colour
uniform vec3 LightDirection;    // Light direction from light

// Point Light Data
uniform vec3 PointLightColor;       // Diffuse light colour
uniform vec3 PointLightPosition;    // Light direction from light


void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    // Lambert term, with L reveresed to face towards the light
    float lambertTerm = max(0, min(1, dot(N, -L)));

    vec3 V = normalize(CameraPosition - vPosition.xyz);

    vec3 R = reflect(L, N);

    float specularTerm = pow(max(0, dot(R, V)), specularPower);

    vec3 Ambient = AmbientColor * Ka;
    vec3 Diffuse = LightColor * Kd * lambertTerm;
    vec3 specular = LightColor * Ks * specularTerm;

    FragColor = vec4(Ambient + Diffuse + specular, 1);
}