#version 330 core

in vec2 iUV;
in float iNL;

out vec4 pixelColor;

uniform sampler2D tex;

void main()
{
	vec4 color = texture(tex, iUV);
	pixelColor = vec4(color.rgb * iNL, color.a);
}