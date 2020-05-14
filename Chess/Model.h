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

	static const Model *board, *boardBorder, *pawn, *bishop, *knightWhite, *knightBlack, *rook, *king, *queen;

private:
	static Model* fromOBJFile(const std::string&);
};