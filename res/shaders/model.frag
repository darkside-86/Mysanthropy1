#version 330 core
out vec4 color;

in vec2 v2texCoords;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

#define NUM_MATERIALS 8
uniform Material u_material[NUM_MATERIALS];

void main()
{
    color = texture(u_material[0].diffuse, v2texCoords);
}