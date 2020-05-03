#pragma once

#include <glm/glm.hpp>

#include "Position.h"
#include "Object.h"


class Piece
{
	const Object *_object;
	glm::vec3 _position;
	float _opacity;

public:
	Piece(const Object *object, const glm::vec3&, float = 1.0f);

	void render(const ShaderProgram*, const glm::mat4&) const;

	const glm::vec3& position() const { return _position; }
	void setPosition(const glm::vec3 &position) { _position = position; }
	void setPosition(const Position &position) { _position = position; }
	void setOpacity(float opacity) { _opacity = opacity; }
};

