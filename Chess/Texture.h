#pragma once

#include <GL/glew.h>
#include <array>


constexpr int
SHADOW_WIDTH = 1024,
SHADOW_HEIGHT = 1024,
LIGHT_COUNT = 2;


class Texture
{
public:
	static void loadTextures();
	static void deleteTextures();

	static GLuint board, white, black, draganddrop, metalSpec;
	static std::array<GLuint, LIGHT_COUNT> shadowMap, shadowMapFBO;

private:
	static GLuint fromPNGFile(const char*);
	static GLuint forShadowMapping(GLuint&);
};

