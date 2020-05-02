#pragma once

#include "Model.h"
#include "Texture.h"
#include "ShaderProgram.h"


class Object
{
	const Model *_model;
	const GLuint _texture;

public:
	Object(const Model*, GLuint);

	void render(const ShaderProgram*) const;
};

