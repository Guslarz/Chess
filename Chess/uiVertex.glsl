#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 UV;

out vec2 iTexCoord0;

void main()
{
	gl_Position = vec4(vertex, 0.0f, 1.0f);
	iTexCoord0 = UV;
}