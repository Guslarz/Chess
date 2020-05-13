#version 330 core

uniform mat4 P, V, M;
uniform vec4 light = vec4(0.0f, 10.0f, -2.0f, 1.0f);

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 UV;

out vec2 iTexCoord0;
out vec4 l, n, v;

void main()
{
	vec4 tmp = V * M * vertex;
	l = normalize(V * light - tmp);
	v = normalize(-tmp);
	n = normalize(V * M * normal);

	gl_Position = P * V * M * vertex;
	iTexCoord0 = UV;
}