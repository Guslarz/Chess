#version 330 core

uniform mat4 PVM;

layout(location = 0) in vec4 vertex;

void main()
{
	gl_Position = PVM * vertex;
}