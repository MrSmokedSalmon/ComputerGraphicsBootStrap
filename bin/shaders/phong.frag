// A phong shader
#version 410

in vec3 vNormal;    // Surface normal from mesh
in vec4 vPosition;          // World-space surface position from mesh

out vec4 FragColor;

uniform vec3 CameraPosition; // World-space position of the camera

// MTL Data
uniform vec3 Ka; // Ambient material colour
uniform vec3 Kd; // Diffuse material colour
uniform vec3 Ks; // Specular material colour
uniform float specularPower; // The material specular power

// Light Data
uniform vec3 AmbientColor; // Ambient light colour
uniform vec3 LightColor; // Diffuse light colour
uniform vec3 LightDirection;    // Light direction from light

void main(){
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    // Lambert term, with L reveresed to face towards the light
    float lambertTerm = max(0, min(1, dot(N, -L)));

    vec3 V = normalize(CameraPosition - vPosition.xyz);
    vec3 R = reflect(L, N);

    float specularTerm = pow(min(0.0f, dot(R, V)), specularPower);

    vec3 Ambient = AmbientColor * Ka;
    vec3 Diffuse = LightColor * Kd * lambertTerm;
    vec3 Specular = LightColor * Ks * specularTerm;

    vec4 newColor = vec4(Ambient + Diffuse + Specular, 1);

    FragColor = newColor;
}