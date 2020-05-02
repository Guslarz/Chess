#include "Model.h"

#include <fstream>

#include <glm/glm.hpp>


const Model *Model::cube, *Model::board, *Model::pawn, *Model::bishop, *Model::knight, *Model::rook, *Model::king, *Model::queen;



Model::Model(const std::vector<float> &vertices, const std::vector<float> &normals, const std::vector<float> &uvs) :
	_vertices(vertices), _normals(normals), _uvs(uvs), _count(static_cast<GLsizei>(vertices.size() / 4))
{}


void Model::render() const
{
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, _vertices.data());
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, _normals.data());
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, _uvs.data());
	glDrawArrays(GL_TRIANGLES, 0, _count);
	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


void Model::loadModels()
{
	cube = fromOBJFile("models/cube.obj");
	board = fromOBJFile("models/board.obj");
	pawn = fromOBJFile("models/pawn.obj");
	bishop = fromOBJFile("models/bishop.obj");
	knight = fromOBJFile("models/knight.obj");
	rook = fromOBJFile("models/rook.obj");
	king = fromOBJFile("models/king.obj");
	queen = fromOBJFile("models/queen.obj");
}


void Model::deleteModels()
{
	delete cube;
	delete board;
	delete pawn;
	delete bishop;
	delete knight;
	delete rook;
	delete king;
	delete queen;
}


Model* Model::fromOBJFile(const std::string &filename)
{
	std::vector<glm::vec4> tmpVertices;
	std::vector<glm::vec3> tmpNormals;
	std::vector<glm::vec2> tmpUvs;
	std::vector<unsigned int> vertexIndices, normalIndices, uvIndices;
	std::ifstream file(filename);
	std::string head, line;
	float x, y, z;
	unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

	while (file >> head) {
		if (head == "v") {
			file >> x >> y >> z;
			tmpVertices.push_back(glm::vec4(x, y, z, 1.0f));
		}
		else if (head == "vt") {
			file >> x >> y;
			tmpUvs.push_back(glm::vec2(x, y));
		}
		else if (head == "vn") {
			file >> x >> y >> z;
			tmpNormals.push_back(glm::vec3(x, y, z));
		}
		else if (head == "f") {
			std::getline(file, line);
			if (sscanf_s(line.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", vertexIndex, uvIndex, normalIndex, vertexIndex + 1, 
				uvIndex + 1, normalIndex + 1, vertexIndex + 2, uvIndex + 2, normalIndex + 2) != 9)
				return nullptr;
			vertexIndices.push_back(vertexIndex[0] - 1);
			vertexIndices.push_back(vertexIndex[1] - 1);
			vertexIndices.push_back(vertexIndex[2] - 1);
			uvIndices.push_back(uvIndex[0] - 1);
			uvIndices.push_back(uvIndex[1] - 1);
			uvIndices.push_back(uvIndex[2] - 1);
			normalIndices.push_back(normalIndex[0] - 1);
			normalIndices.push_back(normalIndex[1] - 1);
			normalIndices.push_back(normalIndex[2] - 1);
		}
	}

	std::vector<float> vertices, normals, uvs;
	for (auto &index : vertexIndices) {
		auto &vertex = tmpVertices[index];
		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);
		vertices.push_back(vertex.w);
	}
	for (auto &index : normalIndices) {
		auto &normal = tmpNormals[index];
		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
	}
	for (auto &index : uvIndices) {
		auto &uv = tmpUvs[index];
		uvs.push_back(uv.x);
		uvs.push_back(uv.y);
	}

	file.close();
	return new Model(vertices, normals, uvs);
}