#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <array>

#include "Piece.h"
#include "Move.h"
#include "ShaderProgram.h"


class Board
{
	std::vector<Piece*> _pieces;
	std::array<Piece*, 64> _board;
	std::vector<Piece*> _captured;
	const ShaderProgram *_shaderProgram;
	const glm::mat4 &_P, &_V, &_M;
	const Model *_model;

public:
	Board(const ShaderProgram*, const glm::mat4&, const glm::mat4&, const glm::mat4&);
	~Board();

	void render() const;
	void applyMove(const Move*);

private:
	void addPiece(Piece*);
	void capturePieceAt(const Position&);
	void movePiece(const Position&, const Position&);
};

