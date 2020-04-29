#include "Piece.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


Piece::Piece(const Model *model, const Position &position) :
	_model(model), _position(position)
{}


void Piece::render(const ShaderProgram *shaderProgram, const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &M) const
{
	glm::mat4 PVM = P * V * glm::scale(glm::translate(M, glm::vec3(_position.x(), .05f, _position.y())), glm::vec3(0.25f, 1.0f, 0.25f));
	glUniformMatrix4fv(shaderProgram->getUniform("PVM"), 1, GL_FALSE, glm::value_ptr(PVM));
	_model->render();
}