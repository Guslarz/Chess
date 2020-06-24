#include "Object.h"

#include "Move.h"


std::array<std::array<Object*, 6>, 2> Object::piece;


Object::Object(const Model *model, GLuint diffuse, GLuint specular) :
	_model(model), _diffuse(diffuse), _specular(specular)
{}


void Object::render() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuse);
	glUniform1i(ShaderProgram::objectShader->getUniform("texSampler0"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _specular);
	glUniform1i(ShaderProgram::objectShader->getUniform("texSampler1"), 1);
	_model->render();
}


void Object::renderShadow() const
{
	_model->renderShadow();
}


void Object::loadObjects()
{
	piece[WHITE][KING] = new Object(Model::king, Texture::white, Texture::whiteSpec);
	piece[WHITE][QUEEN] = new Object(Model::queen, Texture::white, Texture::whiteSpec);
	piece[WHITE][ROOK] = new Object(Model::rook, Texture::white, Texture::whiteSpec);
	piece[WHITE][BISHOP] = new Object(Model::bishop, Texture::white, Texture::whiteSpec);
	piece[WHITE][KNIGHT] = new Object(Model::knightWhite, Texture::white, Texture::whiteSpec);
	piece[WHITE][PAWN] = new Object(Model::pawn, Texture::white, Texture::whiteSpec);
	piece[BLACK][KING] = new Object(Model::king, Texture::black, Texture::blackSpec);
	piece[BLACK][QUEEN] = new Object(Model::queen, Texture::black, Texture::blackSpec);
	piece[BLACK][ROOK] = new Object(Model::rook, Texture::black, Texture::blackSpec);
	piece[BLACK][BISHOP] = new Object(Model::bishop, Texture::black, Texture::blackSpec);
	piece[BLACK][KNIGHT] = new Object(Model::knightBlack, Texture::black, Texture::blackSpec);
	piece[BLACK][PAWN] = new Object(Model::pawn, Texture::black, Texture::blackSpec);
}


void Object::deleteObjects()
{
	for (auto &side : piece)
		for (auto object : side)
			delete object;
}