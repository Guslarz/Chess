#version 330 core

#define LIGHT_COUNT 2

in vec2 iTexCoord0;
in vec4 l[LIGHT_COUNT], v, n, shadowCoord[LIGHT_COUNT];

out vec4 pixelColor;

uniform sampler2D texSampler0, texSampler1, shadowMap[LIGHT_COUNT];
uniform float alpha = 1.0f;


void main()
{
	vec4 vnorm = normalize(v),
		nnorm = normalize(n);
	vec4 lnorm[LIGHT_COUNT], rnorm[LIGHT_COUNT];
	vec4 texColor0 = texture(texSampler0, iTexCoord0),
		texColor1 = texture(texSampler1, iTexCoord0);
	
	vec4 kd = texColor0,
		ks = texColor1;
	vec4 tmp[2];
		
	vec2 texelSize = 3.0 / textureSize(shadowMap[0], 0);

	pixelColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < LIGHT_COUNT; ++i) {
		vec4 lnorm = normalize(l[i]);
		vec4 rnorm = reflect(-lnorm, nnorm);
		
		float nl = clamp(dot(nnorm, lnorm), 0, 1);
		float rv = pow(clamp(dot(rnorm, vnorm), 0, 1), 50);

		float visibility = 1.0f;
		float bias =  clamp(0.005 * tan(acos(nl)), 0, 0.01);
		for (float x = -1.0f; x <= 1.0f; ++x) {
			for (float y = -1.0f; y <= 1.0f; ++y){
				if (texture(shadowMap[i], shadowCoord[i].xy + vec2(x, y) * texelSize).z  <  shadowCoord[i].z - bias)
					visibility -= 0.11f;
			}
		}
		if (visibility < 0.0f) 
			visibility = 0.0f;

		pixelColor += vec4(kd.rgb * nl * visibility, kd.a * alpha) + vec4(ks.rgb * rv, 0);
	}

	pixelColor /= float(LIGHT_COUNT);
}