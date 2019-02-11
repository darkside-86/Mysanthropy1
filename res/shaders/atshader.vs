#version 330 core
// vertex position
layout (location=0) in vec3 position;
// 4 component RGBA normalized color
layout (location=1) in vec4 color;
// tex coords
layout (location=2) in vec2 texCoords;
// normals for lighting
layout (location=3) in vec3 normal;

out vec2 v2texCoords;
out vec4 v4color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    v2texCoords = texCoords;
    v4color = color;
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
}
