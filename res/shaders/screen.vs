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

void main()
{
    v2texCoords = texCoords;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}  