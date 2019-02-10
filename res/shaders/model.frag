#version 330 core
out vec4 color;

in vec2 v2texCoords;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// uniform sampler2D texture_diffuse1;
uniform Material u_material[1];

void main()
{    
    color = vec4(texture(u_material[0].diffuse, v2texCoords).rgb, 1.0);
}