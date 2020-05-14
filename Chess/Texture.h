#pragma once

#include <GL/glew.h>


constexpr int
SHADOW_WIDTH = 1024,
SHADOW_HEIGHT = 1024;


class Texture
{
public:
	static GLuint fromPNGFile(const char*);
	static void loadTextures();
	static void deleteTextures();

	static GLuint board, white, black, draganddrop, depth, depthFBO;
};

