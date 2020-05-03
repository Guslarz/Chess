#version 330 core

uniform mat4 P, V, M;

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 UV;

out vec2 iUV;

void main()
{
	gl_Position = P * V * M * vertex;
	iUV = UV;
}