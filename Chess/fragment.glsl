#version 330 core

in vec2 iUV;
in float iNL;

out vec4 pixelColor;

uniform sampler2D tex;
uniform float alpha = 1.0f;

void main()
{
	vec4 color = texture(tex, iUV);
	pixelColor = vec4(color.rgb * iNL, alpha);
}