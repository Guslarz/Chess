#include "Move.h"


Move::Move(Side side) :
	_side(side)
{}


NormalMove::NormalMove(Side side, const Position &from, const Position &to) :
	Move(side), _from(from), _to(to)
{}


NormalMove::operator std::string() const
{
	return std::string("Normal Move (") + std::string(_from) + std::string(", ") + 
		std::string(_to) + std::string(")");
}


CastlingMove::CastlingMove(Side side, Type type) :
	Move(side), _type(type)
{
	int king_x, rook_x_from, rook_x_to, y;
	if (side == WHITE) y = 0;
	else y = 7;
	if (type == KINGSIDE) {
		king_x = 6;
		rook_x_from = 7;
		rook_x_to = 5;
	}
	else {
		king_x = 2;
		rook_x_from = 0;
		rook_x_to = 3;
	}
	_kingFrom = Position(4, y);
	_kingTo = Position(king_x, y);
	_rookFrom = Position(rook_x_from, y);
	_rookTo = Position(rook_x_to, y);
}


CastlingMove::operator std::string() const
{
	return std::string("Castling Move (") + std::string(_kingFrom) + std::string(", ") +
		std::string(_kingTo) + std::string(", ") + std::string(_rookFrom) + std::string(", ") +
		std::string(_rookTo) + std::string(")");
}


CaptureMove::CaptureMove(Side side, const Position &from, const Position &to) :
	NormalMove(side, from, to)
{}


CaptureMove::operator std::string() const
{
	return std::string("Capture Move (") + std::string(_from) + std::string(", ") + 
		std::string(_to) + std::string(")");
}


EnPassantCaptureMove::EnPassantCaptureMove(Side side, const Position &from, const Position &to) :
	CaptureMove(side, from, to), NormalMove(side, from, to), _target(to.x(), to.y() + 2 * side - 1)
{}


EnPassantCaptureMove::operator std::string() const
{
	return std::string("En Passant Capture Move (") + std::string(_from) + std::string(", ") +
		std::string(_to) + std::string(", ") + std::string(target()) + std::string(")");
}


PromotionMove::PromotionMove(Side side, const Position &from, const Position &to, PieceType promotion) : 
	NormalMove(side, from, to), _promotion(promotion)
{}


PromotionMove::operator std::string() const
{
	return std::string("Promotion Move (") + std::string(_from) + std::string(", ") +
		std::string(_to) + std::string(", ") + static_cast<char>('0' + _promotion) + std::string(")");
}


CapturePromotionMove::CapturePromotionMove(Side side, const Position &from, const Position &to, PieceType promotion) :
	CaptureMove(side, from, to), PromotionMove(side, from, to, promotion), NormalMove(side, from, to)
{}


CapturePromotionMove::operator std::string() const
{
	return std::string("Capture Promotion Move (") + std::string(_from) + std::string(", ") +
		std::string(_to) + std::string(", ") + static_cast<char>('0' + promotion()) + std::string(")");
}