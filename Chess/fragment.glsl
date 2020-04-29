#version 330 core

out vec4 pixelColor;

uniform vec4 color = vec4(.0f, .0f, .0f, 1.0f);


void main()
{
	pixelColor = color;
}