#version 330 core

// vertex position
layout (location=0) in vec3 position;
// 4 component RGBA normalized color
layout (location=1) in vec4 color;
// tex coords
layout (location=2) in vec2 texCoords;
// normals
layout (location=3) in vec3 normal;

// send texture coordinates and color to fragment shader
out vec4 v_color;
out vec2 v_texCoord;

// model view projection matrix
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() 
{
    // apply the matrix to the positon
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
    // set the texture coordinates to their value for fragment shader
    v_texCoord = texCoords;
    // set the color
    v_color = color;
}