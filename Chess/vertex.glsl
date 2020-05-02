#version 330 core

uniform mat4 PVM;

layout(location = 0) in vec4 vertex;
layout(location = 2) in vec2 UV;

out vec2 iUV;

void main()
{
	gl_Position = PVM * vertex;
	iUV = UV;
}