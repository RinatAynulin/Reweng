#version 440 core

layout (location = 0) in vec3 aPos;
layout( location = 1) in vec3 color;
layout( location = 2) in vec2 texPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 outColor;
out vec2 outTexPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    outColor  = color;
    outTexPos = texPos;
}
