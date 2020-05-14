#version 330 core

layout(location = 0) in vec4 vertex;

uniform mat4 depthP, depthV, M;

void main()
{
	gl_Position = depthP * depthV * M * vertex;
}