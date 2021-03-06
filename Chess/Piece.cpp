#include "Piece.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


Piece::Piece(const Object *object, const glm::vec3 &position, float opacity) :
	_object(object), _position(position), _opacity(opacity)
{}


void Piece::render(const glm::mat4 &M) const
{
	glUniformMatrix4fv(ShaderProgram::objectShader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(glm::translate(M, _position)));
	glUniform1f(ShaderProgram::objectShader->getUniform("alpha"), _opacity);
	_object->render();
}


void Piece::renderShadow(const glm::mat4 &M) const
{
	glUniformMatrix4fv(ShaderProgram::depthShader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(glm::translate(M, _position)));
	_object->renderShadow();
}