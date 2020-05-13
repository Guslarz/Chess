#version 330 core

in vec2 iTexCoord0;
in vec4 l, v, n;

out vec4 pixelColor;

uniform sampler2D texSampler0;
uniform float alpha = 1.0f;

void main()
{
	vec4 lnorm = normalize(l),
		vnorm = normalize(v),
		nnorm = normalize(n);
	vec4 rnorm = reflect(-lnorm, nnorm);

	vec4 texColor0 = texture(texSampler0, iTexCoord0);

	vec4 kd = texColor0,
		ks = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	float nl = clamp(dot(nnorm, lnorm), 0, 1);
	float rv = pow(clamp(dot(rnorm, vnorm), 0, 1), 50);

	pixelColor = vec4(kd.rgb * nl, kd.a * alpha) + vec4(ks.rgb * rv, 0);
}