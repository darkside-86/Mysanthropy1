 #version 330 core
out vec4 color;

in vec3 v3normal;
in vec3 v3fragPos;
in vec4 v4color;
in vec2 v2texCoords;

uniform vec3 u_viewPos;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material u_material;

struct Light 
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};
uniform Light u_light;

void main()
{
    vec3 lightDir = normalize(u_light.position - v3fragPos);
    
    // ambient
    vec3 ambient = u_light.ambient * texture(u_material.diffuse, v2texCoords).rgb;

    // diffuse
    vec3 norm = normalize(v3normal);
    float diff = max(dot(v3normal,lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * texture(u_material.diffuse, v2texCoords).rgb;

    // specular
    vec3 viewDir = normalize(u_viewPos - v3fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * spec * texture(u_material.specular, v2texCoords).rgb;

    // spotlight
    float theta = dot(lightDir, normalize(-u_light.direction));
    float epsilon = u_light.cutOff - u_light.outerCutOff;
    float intensity = clamp((theta - u_light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    float distance = length(u_light.position - v3fragPos);
    float attenuation = 1.0 / (u_light.constant + u_light.linear * distance +
                        u_light.quadratic * ( distance * distance ) );

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}