#version 330 core

uniform mat4 P, V, M;
uniform vec4 light = vec4(0, 0, 1, 0);

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 UV;

out vec2 iUV;
out float iNL;

void main()
{
	gl_Position = P * V * M * vertex;
	iUV = UV;

	mat4 G = mat4(inverse(transpose(mat3(M))));
    vec4 n = normalize(V * G * normal);

    iNL = clamp(dot(n, light), 0, 1);
    iUV = UV;
}