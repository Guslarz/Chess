#include "Piece.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


Piece::Piece(const Object *object, const glm::vec3 &position, float opacity) :
	_object(object), _position(position), _opacity(opacity)
{}


void Piece::render(const ShaderProgram *shaderProgram, const glm::mat4 &M) const
{
	glUniformMatrix4fv(shaderProgram->getUniform("M"), 1, GL_FALSE, glm::value_ptr(glm::translate(M, _position)));
	glUniform1f(shaderProgram->getUniform("alpha"), _opacity);
	_object->render(shaderProgram);
}