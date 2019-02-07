#version 330 core

in vec4 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture;
uniform bool u_useTexture;
uniform bool u_useColorBlending;

out vec4 color;

void main() 
{
    if(u_useTexture)
    {
        if(u_useColorBlending)
            color = texture(u_texture, v_texCoord) * v_color;
        else
            color = texture(u_texture, v_texCoord);
    }
    else
    {
        color = v_color;
    }

}
