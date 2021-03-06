#version 330 core

#define LIGHT_COUNT 2

uniform mat4 P, V, M, depthP, depthV[LIGHT_COUNT];
uniform vec4 light[LIGHT_COUNT];

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 UV;

out vec2 iTexCoord0;
out vec4 l[LIGHT_COUNT], n, v, shadowCoord[LIGHT_COUNT];

void main()
{
	vec4 tmp = V * M * vertex;
	v = normalize(-tmp);
	n = normalize(V * M * normal);

	gl_Position = P * V * M * vertex;
	iTexCoord0 = UV;

	mat4 biasMatrix = mat4(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);
	
	for (int i = 0; i < LIGHT_COUNT; ++i) {
		l[i] = normalize(V * light[i] - tmp);
		shadowCoord[i] = biasMatrix * depthP * depthV[i] * M * vertex;
	}
}