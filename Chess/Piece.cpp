#include "Piece.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


Piece::Piece(const Model *model, GLuint texture, const glm::vec3 &position) :
	_model(model), _position(position), _texture(texture)
{}


void Piece::render(const ShaderProgram *shaderProgram, const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &M) const
{
	glm::mat4 PVM = P * V * glm::translate(glm::scale(glm::translate(M, _position), glm::vec3(0.5f, 0.5f, 0.5f)), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(shaderProgram->getUniform("PVM"), 1, GL_FALSE, glm::value_ptr(PVM));
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, _texture);
	glUniform1i(shaderProgram->getUniform("tex"), 0);
	_model->render();
}