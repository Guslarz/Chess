#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>


class Model
{
	GLsizei _count;
	const std::vector<float> _vertices, _normals, _uvs;

public:
	Model(const std::vector<float>&, const std::vector<float>&, const std::vector<float>&);
	void render() const;

	static void loadModels();
	static void deleteModels();

	static const Model *cube, *board, *boardBorder, *pawn, *bishop, *knightWhite, *knightBlack, *rook, *king, *queen, *floor, *cylinder;

private:
	static Model* fromOBJFile(const std::string&, float = 1.0f);
};