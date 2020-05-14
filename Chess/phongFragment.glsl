#version 330 core

in vec2 iTexCoord0;
in vec4 l, v, n, shadowCoord;

out vec4 pixelColor;

uniform sampler2D texSampler0, shadowMap;
uniform float alpha = 1.0f, bias = 0.005;

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
	
	float bias =  clamp(bias * tan(acos(nl)), 0, 0.01);
	float visibility = 1.0f;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (float x = -1.0f; x <= 1.0f; x += 0.2f) {
		for (float y = -1.0f; y <= 1.0f; y += 0.2f) {
		if (texture(shadowMap, shadowCoord.xy + vec2(x, y) * texelSize).z  <  shadowCoord.z - bias)
			visibility -= 0.01;
		}
	}

	pixelColor = vec4(kd.rgb * nl * visibility, kd.a * alpha) + vec4(ks.rgb * rv, 0);
}