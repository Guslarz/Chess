#version 330 core

in vec2 iUV;

out vec4 pixelColor;

uniform sampler2D tex;

void main()
{
	pixelColor = texture(tex, iUV);
}