#version 330 core
out vec4 color;

in vec2 v2texCoords;
in vec4 v4color;

uniform sampler2D u_texture1;

void main()
{    
    color = texture(u_texture1, v2texCoords);
}