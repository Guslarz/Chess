#pragma once

#include <glm/glm.hpp>

#include "Position.h"
#include "Object.h"


class Piece
{
	Object _object;
	glm::vec3 _position;

public:
	Piece(const Model*, GLuint, const glm::vec3&);

	void render(const ShaderProgram*, const glm::mat4&) const;

	const glm::vec3& position() const { return _position; }
	void setPosition(const glm::vec3 &position) { _position = position; }
	void setPosition(const Position &position) { _position = position; }
};

