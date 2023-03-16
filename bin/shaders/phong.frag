// A phong shader
#version 410

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 1

in vec3 vNormal;    // Surface normal from mesh
in vec4 vPosition;          // World-space surface position from mesh

out vec4 FragColor;

uniform vec3 CameraPosition; // World-space position of the camera

uniform mat4 encodedPL[NR_POINT_LIGHTS];
uniform PointLight PointLights[NR_POINT_LIGHTS];

uniform bool enablePoint = false;

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


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularPower);

    // Attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
                             light.quadratic *  (dist * dist));

    // Combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

PointLight DecodePoint(mat4 light)
{
    PointLight newLight;

    newLight.position = light[0].xyz;
    newLight.ambient = light[1].xyz;
    newLight.diffuse = light[2].xyz;
    newLight.specular = light[3].xyz;

    newLight.constant = light[0].w;
    newLight.linear = light[1].w;
    newLight.quadratic = light[2].w;

    return newLight;
}

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    // Lambert term, with L reveresed to face towards the light
    float lambertTerm = max(0, min(1, dot(N, -L)));

    vec3 V = normalize(CameraPosition - vPosition.xyz);

    vec3 R = reflect(L, N);

    float specularTerm = pow(max(0, dot(R, V)), specularPower);

    vec3 ambient = AmbientColor * Ka;
    vec3 diffuse = LightColor * Kd * lambertTerm;
    vec3 specular = LightColor * Ks * specularTerm;

    vec3 result = ambient + diffuse + specular;

    if (enablePoint)
    {
        for (int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            PointLight pl = DecodePoint(encodedPL[i]);
            result += CalcPointLight(pl, vNormal, vPosition.xyz, CameraPosition);
        }
    }

    FragColor = vec4(result, 1);
}