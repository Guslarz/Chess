#version 330 core

uniform sampler2D texSampler0;
uniform vec4 color;

in vec2 iTexCoord0;

out vec4 pixelColor;

void main()
{
	pixelColor = color * vec4(1.0, 1.0, 1.0, texture(texSampler0, iTexCoord0).r);
}