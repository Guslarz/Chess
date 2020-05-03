#include "Piece.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


Piece::Piece(const Object *object, const glm::vec3 &position) :
	_object(object), _position(position)
{}


void Piece::render(const ShaderProgram *shaderProgram, const glm::mat4 &M) const
{
	glUniformMatrix4fv(shaderProgram->getUniform("M"), 1, GL_FALSE, glm::value_ptr(glm::translate(M, _position)));
	_object->render(shaderProgram);
}