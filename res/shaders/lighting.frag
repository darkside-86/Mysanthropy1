#version 330 core

out vec4 color;

#define NR_POINT_LIGHTS 1

struct Material
{
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v3normal;
in vec3 v3fragPos;
in vec4 v4color;
in vec2 v2texCoords;

uniform vec3 u_viewPos;
uniform DirLight u_dirLight;
uniform PointLight u_pointLights[NR_POINT_LIGHTS];
uniform SpotLight u_spotLight;
uniform Material u_material;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(v3normal);
    vec3 viewDir = normalize(u_viewPos - v3fragPos);
    // phase 1: directional lighting
    vec3 result = CalcDirLight(u_dirLight, norm, viewDir);
    // phase 2: point lights
    /*for(int i=0; i < NR_POINT_LIGHTS; ++i)
    {
        result += CalcPointLight(u_pointLights[i], norm, v3fragPos, viewDir);
    }*/
    // phase 3: spot light
    // result += CalcSpotLight(u_spotLight, norm, v3fragPos, viewDir);

    color = (texture(texture_diffuse1, v2texCoords)) * (texture(texture_specular1, v2texCoords)) 
            + (texture(texture_diffuse2, v2texCoords)) * (texture(texture_specular2, v2texCoords)) + vec4(result, 1.0);
    // color = texture(texture_diffuse1, v2texCoords);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    // combine results
    vec3 ambient = light.ambient * u_material.ambient;
    vec3 diffuse = light.diffuse * (diff * u_material.diffuse);
    vec3 specular = light.specular * (spec * u_material.specular);
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * u_material.ambient;
    vec3 diffuse = light.diffuse * (diff * u_material.diffuse);
    vec3 specular = light.specular * (spec * u_material.specular);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * u_material.ambient;
    vec3 diffuse = light.diffuse * (diff * u_material.diffuse);
    vec3 specular = light.specular * (spec * u_material.specular);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}