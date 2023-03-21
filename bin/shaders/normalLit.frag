// A shader
#version 410

in vec3 vNormal;    // Surface normal from mesh
in vec4 vPosition;  // World-space surface position from mesh
in vec2 vTexCoord;  // Texture coordinate
in vec3 vTangent;   
in vec3 vBiTangent;

out vec4 FragColor;

uniform vec3 CameraPosition; // World-space position of the camera
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

// MTL Data
uniform vec3 Ka; // Ambient material colour
uniform vec3 Kd; // Diffuse material colour
uniform vec3 Ks; // Specular material colour
uniform float specularPower; // The material specular power

// Light Data
uniform vec3 AmbientColor;      // Ambient light colour
uniform vec3 LightColor;        // Diffuse light colour
uniform vec3 LightDirection;    // Light direction from light

void main()
{
    vec3 N = normalize(vNormal);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);
    vec3 L = normalize(LightDirection);

    mat3 TBN = mat3(T,B,N);

    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture, vTexCoord).rgb;

    N = TBN * (texNormal * 2 - 1);

    // Lambert term, with L reveresed to face towards the light
    float lambertTerm = max(0, min(1, dot(N, -L)));

    vec3 V = normalize(CameraPosition - vPosition.xyz);

    vec3 R = reflect(L, N);

    float specularTerm = pow(max(0, dot(R, V)), specularPower);

    vec3 ambient = AmbientColor * Ka * texDiffuse;
    vec3 diffuse = LightColor *  Kd * texDiffuse * lambertTerm ;
    vec3 specular = LightColor * Ks * texSpecular * specularTerm;

    //FragColor = vec4(ambient + diffuse + specular, 1);
    FragColor = vec4(N, 1);
}