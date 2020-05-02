#include "Piece.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


Piece::Piece(const Model *model, GLuint texture, const glm::vec3 &position) :
	_object(model, texture), _position(position)
{}


void Piece::render(const ShaderProgram *shaderProgram, const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &M) const
{
	glm::mat4 PVM = P * V * glm::translate(M, _position);
	glUniformMatrix4fv(shaderProgram->getUniform("PVM"), 1, GL_FALSE, glm::value_ptr(PVM));
	_object.render(shaderProgram);
}