#pragma once

#include <vector>

#include <GL/glew.h>


class Model
{
	GLsizei _size;
	const std::vector<float> _vertices;

public:
	Model(const std::vector<float>&);
	void render() const;

	static void loadModels();

	static const Model *cube, *rect, *triangle;
};