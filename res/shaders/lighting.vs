#version 330 core
// vertex position
layout (location=0) in vec3 position;
// 4 component RGBA normalized color
layout (location=1) in vec4 color;
// tex coords
layout (location=2) in vec2 texCoords;
// normals for lighting
layout (location=3) in vec3 normal;

out vec3 v3normal;
out vec3 v3fragPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
    v3fragPos = vec3(u_model * vec4(position, 1.0));
    v3normal = mat3(transpose(inverse(u_model))) * normal;
} 
