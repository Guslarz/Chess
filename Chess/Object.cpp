#include "Object.h"


Object::Object(const Model *model, GLuint texture) :
	_model(model), _texture(texture)
{}


void Object::render(const ShaderProgram *shaderProgram) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glUniform1i(shaderProgram->getUniform("tex"), 0);
	_model->render();
}