#version 330 core

in vec2 v2_texCoords;

uniform sampler2D u_texture;

out vec4 color;

void main() 
{
    color = texture(u_texture, v2_texCoords);
}
