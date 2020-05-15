#include "Texture.h"

#include <vector>

#include "lodepng.h"


GLuint Texture::board, Texture::white, Texture::black, Texture::draganddrop;
std::array<GLuint, LIGHT_COUNT> Texture::shadowMap, Texture::shadowMapFBO;


void Texture::loadTextures()
{
	board = fromPNGFile("textures/board.png");
	white = fromPNGFile("textures/white.png");
	black = fromPNGFile("textures/black.png");
	draganddrop = fromPNGFile("textures/draganddrop.png");

	for (size_t i = 0; i < LIGHT_COUNT; ++i)
		shadowMap[i] = forRendering(shadowMapFBO[i]);
}


void Texture::deleteTextures()
{
	glDeleteTextures(1, &board);
	glDeleteTextures(1, &white);
	glDeleteTextures(1, &black);
	glDeleteTextures(1, &draganddrop);

	glDeleteTextures(LIGHT_COUNT, &shadowMap[0]);
	glDeleteFramebuffers(LIGHT_COUNT, &shadowMapFBO[0]);
}


GLuint Texture::fromPNGFile(const char *filename)
{
	std::vector<unsigned char> img;
	unsigned width, height;
	unsigned err = lodepng::decode(img, width, height, filename);

	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	return tex;
}


GLuint Texture::forRendering(GLuint &framebuffer)
{
	GLuint tex;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_COMPARE_REF_TO_TEXTURE);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex, 0);
	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer error");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return tex;
}