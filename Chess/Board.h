#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <array>

#include "Move.h"
#include "ShaderProgram.h"
#include "Animation.h"


class Board
{
	std::vector<Piece*> _pieces;
	std::array<Piece*, 64> _board;
	std::vector<Piece*> _captured;
	std::vector<Animation*> _animations;
	float _time;

	const ShaderProgram *_shaderProgram;
	const glm::mat4 &_P, &_V, &_M;

	Object _object;

public:
	Board(const ShaderProgram*, const glm::mat4&, const glm::mat4&, const glm::mat4&);
	~Board();

	void render() const;
	void applyAnimations();
	void applyMove(const Move*);
	void addTime(float time) { _time += time; }
	void finishAnimations();

	bool finished() const;

private:
	void addPiece(const Object*, unsigned int, unsigned int);
	void capturePieceAt(const Position&);
	void movePiece(const Position&, const Position&);
	void makeCastlingMove(const Position&, const Position&, const Position&, const Position&);
};

