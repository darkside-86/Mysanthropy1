 #version 330 core
out vec4 color;

in vec3 v3normal;
in vec3 v3fragPos;

uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform vec3 u_objectColor;
uniform vec3 u_lightColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;

    // diffuse
    vec3 norm = normalize(v3normal);
    vec3 lightDir = normalize(u_lightPos - v3fragPos);
    float diff = max(dot(v3normal,lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPos - v3fragPos);
    vec3 reflectDir = reflect(-u_lightPos, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    color = vec4(result, 1.0);
}