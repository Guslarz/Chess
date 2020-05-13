#pragma once

#include <GL/glew.h>


class Texture
{
public:
	static GLuint fromPNGFile(const char*);
	static void loadTextures();
	static void deleteTextures();

	static GLuint cube, board, white, black, floor, fog, draganddrop;
};

