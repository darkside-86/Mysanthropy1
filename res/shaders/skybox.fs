#version 330 core
out vec4 color;

in vec3 v2texCoords;

uniform samplerCube u_skybox;

void main()
{    
    color = texture(u_skybox, v2texCoords);
}