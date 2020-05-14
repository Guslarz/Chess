#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <array>

#include "Move.h"
#include "Animation.h"


class Board
{
	std::vector<Piece*> _pieces;
	std::array<Piece*, 64> _board;
	std::vector<Piece*> _captured;
	std::vector<Animation*> _animations;
	float _time;

	Object _boardObject, _boardBorderObject;

public:
	Board();
	~Board();

	void render(const glm::mat4&) const;
	void renderShadow(const glm::mat4&) const;
	void applyAnimations();
	void applyMove(const Move*);
	void undoMove(const Move*);
	void addTime(float time) { _time += time; }
	void finishAnimations();

	bool finished() const;

private:
	Piece* addPiece(const Object*, unsigned int, unsigned int);
	void capturePieceAt(const Position&);
	void movePiece(const Position&, const Position&);
	void makeCastlingMove(const Position&, const Position&, const Position&, const Position&);
	void makePromotionMove(const Position&, Side, PieceType);
};

