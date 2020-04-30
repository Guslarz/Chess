#pragma once

#include <glm/glm.hpp>

#include "Model.h"
#include "Position.h"
#include "ShaderProgram.h"


class Piece
{
	const Model *_model;
	//texture;
	Position _position;

public:
	Piece(const Model*, const Position&);

	void render(const ShaderProgram*, const glm::mat4&, const glm::mat4&, const glm::mat4&) const;
	void renderForceM(const ShaderProgram*, const glm::mat4&, const glm::mat4&, const glm::mat4&) const;

	const Position& position() const { return _position; }
	void setPosition(const Position &position) { _position = position; }
};

