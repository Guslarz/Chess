#include "Object.h"

#include "Move.h"


std::array<std::array<Object*, 6>, 2> Object::piece;


Object::Object(const Model *model, GLuint texture) :
	_model(model), _texture(texture)
{}


void Object::render() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glUniform1i(ShaderProgram::objectShader->getUniform("texSampler0"), 0);
	_model->render();
}


void Object::renderShadow() const
{
	_model->renderShadow();
}


void Object::loadObjects()
{
	piece[WHITE][KING] = new Object(Model::king, Texture::white);
	piece[WHITE][QUEEN] = new Object(Model::queen, Texture::white);
	piece[WHITE][ROOK] = new Object(Model::rook, Texture::white);
	piece[WHITE][BISHOP] = new Object(Model::bishop, Texture::white);
	piece[WHITE][KNIGHT] = new Object(Model::knightWhite, Texture::white);
	piece[WHITE][PAWN] = new Object(Model::pawn, Texture::white);
	piece[BLACK][KING] = new Object(Model::king, Texture::black);
	piece[BLACK][QUEEN] = new Object(Model::queen, Texture::black);
	piece[BLACK][ROOK] = new Object(Model::rook, Texture::black);
	piece[BLACK][BISHOP] = new Object(Model::bishop, Texture::black);
	piece[BLACK][KNIGHT] = new Object(Model::knightBlack, Texture::black);
	piece[BLACK][PAWN] = new Object(Model::pawn, Texture::black);
}


void Object::deleteObjects()
{
	for (auto &side : piece)
		for (auto object : side)
			delete object;
}