#pragma once

#include <GL/glew.h>
#include <array>


constexpr int
SHADOW_WIDTH = 2048,
SHADOW_HEIGHT = 2048,
LIGHT_COUNT = 2;


class Texture
{
public:
	static void loadTextures();
	static void deleteTextures();

	static GLuint board, white, whiteSpec, black, blackSpec, draganddrop, metalSpec, brown;
	static std::array<GLuint, LIGHT_COUNT> shadowMap, shadowMapFBO;

private:
	static GLuint fromPNGFile(const char*);
	static GLuint forShadowMapping(GLuint&);
};

