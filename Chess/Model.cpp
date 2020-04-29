#include "Model.h"


const Model 
*Model::cube = nullptr,
*Model::rect = nullptr, *Model::triangle = nullptr;



Model::Model(const std::vector<float> &vertices) :
	_size(static_cast<GLsizei>(vertices.size())),
	_vertices(vertices)
{}


void Model::render() const
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, _vertices.data());
	glDrawArrays(GL_TRIANGLES, 0, _size);
	glDisableVertexAttribArray(0);
}


void Model::loadModels()
{
	cube = new Model({ 
		1.0f,-1.0f,-1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,
		-1.0f,-1.0f,-1.0f,1.0f,

		1.0f,-1.0f,-1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,

		-1.0f,-1.0f, 1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,
		1.0f,-1.0f, 1.0f,1.0f,

		-1.0f,-1.0f, 1.0f,1.0f,
		-1.0f, 1.0f, 1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		1.0f,-1.0f, 1.0f,1.0f,
		1.0f,-1.0f,-1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f,-1.0f, 1.0f,1.0f,
		1.0f,-1.0f, 1.0f,1.0f,

		-1.0f, 1.0f, 1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,

		-1.0f, 1.0f, 1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f, 1.0f, 1.0f,1.0f,
		-1.0f,-1.0f, 1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,
		-1.0f, 1.0f, 1.0f,1.0f,

		1.0f,-1.0f, 1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,
		1.0f,-1.0f,-1.0f,1.0f,

		1.0f,-1.0f, 1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f });

	rect = new Model({
		-1.0f, .0f, -1.0f, 1.0f,
		-1.0f, .0f, 1.0f, 1.0f,
		1.0f, .0f, 1.0f, 1.0f,
		-1.0f, .0f, -1.0f, 1.0f,
		1.0f, .0f, 1.0f, 1.0f,
		1.0f, .0f, -1.0f, 1.0f });


}