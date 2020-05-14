#include "Texture.h"

#include <vector>

#include "lodepng.h"


GLuint Texture::board, Texture::white, Texture::black, Texture::draganddrop;


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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	return tex;
}


void Texture::loadTextures()
{
	board = fromPNGFile("textures/board.png");
	white = fromPNGFile("textures/white.png");
	black = fromPNGFile("textures/black.png");
	draganddrop = fromPNGFile("textures/draganddrop.png");
}


void Texture::deleteTextures()
{
	glDeleteTextures(1, &board);
	glDeleteTextures(1, &white);
	glDeleteTextures(1, &black);
	glDeleteTextures(1, &draganddrop);
}