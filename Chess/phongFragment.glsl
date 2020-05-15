#version 330 core

in vec2 iTexCoord0;
in vec4 l[2], v, n, shadowCoord[2];

out vec4 pixelColor;

uniform sampler2D texSampler0, texSampler1, shadowMap[2];
uniform float alpha = 1.0f;


void main()
{
	vec4 vnorm = normalize(v),
		nnorm = normalize(n);
	vec4 lnorm[2], rnorm[2];
	vec4 texColor0 = texture(texSampler0, iTexCoord0),
		texColor1 = texture(texSampler1, iTexCoord0);
	
	vec4 kd = texColor0,
		ks = texColor1;
	vec4 tmp[2];
		
	vec2 texelSize = 1.0 / textureSize(shadowMap[0], 0);

	for (int i = 0; i < 2; ++i) {
		vec4 lnorm = normalize(l[i]);
		vec4 rnorm = reflect(-lnorm, nnorm);
		
		float nl = clamp(dot(nnorm, lnorm), 0, 1);
		float rv = pow(clamp(dot(rnorm, vnorm), 0, 1), 50);

		float visibility = 1.0f;
		float bias =  clamp(0.005 * tan(acos(nl)), 0, 0.01);
		for (float x = -1.0f; x <= 1.0f; x += 0.4f) {
			for (float y = -1.0f; y <= 1.0f; y += 0.4f){
				if (texture(shadowMap[i], shadowCoord[i].xy + vec2(x, y) * texelSize).z  <  shadowCoord[i].z - bias)
					visibility -= 0.02f;
			}
		}
		if (visibility < 0.0f) 
			visibility = 0.0f;

		tmp[i] = vec4(kd.rgb * nl * visibility, kd.a * alpha) + vec4(ks.rgb * rv, 0);
	}

	pixelColor = (tmp[0] + tmp[1]) / 2.0f;
}