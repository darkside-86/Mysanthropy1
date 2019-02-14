#version 330 core
out vec4 color;

in vec2 v2texCoords;
in vec4 v4color;

uniform sampler2D u_texture1;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return ( 2.0 * near * far ) / (far + near - z * (far-near));
}

void main()
{    
    vec4 texColor = texture(u_texture1, v2texCoords);
    if(texColor.a < 0.1)
        discard;
    color = texColor;
    // color = vec4(vec3(gl_FragCoord.z),1.0);
    // float depth = LinearizeDepth(gl_FragCoord.z) / far;
    // color = vec4(vec3(depth),1.0);
}