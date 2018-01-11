#version 440 core

uniform sampler2D texture1;
//uniform sampler2D texture2;

in vec2 outTexPos;

out vec4 FragColor;

void main()
{
  FragColor = texture(texture1, outTexPos);
}
