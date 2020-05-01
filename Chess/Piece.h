#pragma once

#include <glm/glm.hpp>

#include "Model.h"
#include "Position.h"
#include "ShaderProgram.h"


class Piece
{
	const Model *_model;
	//texture;
	glm::vec3 _position;

public:
	Piece(const Model*, const glm::vec3&);

	void render(const ShaderProgram*, const glm::mat4&, const glm::mat4&, const glm::mat4&) const;

	const glm::vec3& position() const { return _position; }
	void setPosition(const glm::vec3 &position) { _position = position; }
	void setPosition(const Position &position) { _position = glm::vec3(static_cast<float>(position.x()), 0.0f, static_cast<float>(position.y())); }
};

