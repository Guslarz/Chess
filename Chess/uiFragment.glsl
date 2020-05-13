#version 330 core

uniform sampler2D texSampler0;

in vec2 iTexCoord0;

out vec4 pixelColor;

void main()
{
	pixelColor = texture(texSampler0, iTexCoord0);
}