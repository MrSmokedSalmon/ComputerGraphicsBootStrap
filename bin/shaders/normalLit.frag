// A shader
#version 410

in vec4 vPosition;  // World-space surface position from mesh
in vec3 vNormal;    // Surface normal from mesh
in vec2 vTexCoord;  // Texture coordinate
in vec3 vTangent;   
in vec3 vBiTangent;

out vec4 FragColor;

uniform vec3 CameraPosition; 

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

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColors[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];

vec3 Diffuse(vec3 direction, vec3 color, vec3 normal)
{
    return color * max(0, dot(normal, -direction));
}

vec3 Specular(vec3 direction, vec3 color, vec3 normal, vec3 view)
{
    vec3 R = reflect(direction, normal);

    float specularTerm = pow(max(0, dot(R, view)), specularPower);
    return specularTerm * color;
}

void main()
{
    vec3 N = normalize(vNormal);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);
    vec3 L = normalize(LightDirection);

    mat3 TBN = mat3(T, B, N);

    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture, vTexCoord).rgb;

    N = TBN * (texNormal * 2 - 1);

    // Lambert term, with L reveresed to face towards the light
    float lambertTerm = max(0, min(1, dot(N, -L)));

    vec3 V = normalize(CameraPosition - vPosition.xyz);

    vec3 R = reflect(L, N);
    
    // Calculate the diffuse value of light from the global source
    vec3 diffuseTotal = Diffuse(L, LightColor, N);

    // Calculate specular value of light from the global source
    vec3 specularTotal = Specular(L, LightColor, N, V);

    for (int i = 0; i < numLights; i++)
    {
        vec3 direction = vPosition.xyz - PointLightPositions[i];
        float distance = length(direction);
        direction = direction / distance;
        
        // Set the lighting intensity with the inverse square law
        vec3 color = PointLightColors[i] / (distance * distance);
        
        diffuseTotal += Diffuse(direction, color, N);
        specularTotal += Specular(direction, color, N, V);
    }

    vec3 ambient = AmbientColor * Ka * texDiffuse;
    vec3 diffuse =  diffuseTotal * Kd * texDiffuse ;
    vec3 specular = specularTotal * Ks * texSpecular;

    FragColor = vec4(ambient + diffuse + specular, 1);
    //FragColor = vec4(N, 1);
}