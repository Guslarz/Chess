#version 330 core

uniform mat4 P, V, M, depthP, depthV;
uniform vec4 light;

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 UV;

out vec2 iTexCoord0;
out vec4 l, n, v, shadowCoord;

void main()
{
	vec4 tmp = V * M * vertex;
	l = normalize(V * light - tmp);
	v = normalize(-tmp);
	n = normalize(V * M * normal);

	gl_Position = P * V * M * vertex;
	iTexCoord0 = UV;

	mat4 biasMatrix = mat4(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);
	shadowCoord = biasMatrix * depthP * depthV * M * vertex;

}