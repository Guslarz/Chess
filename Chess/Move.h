#pragma once

#include "Position.h"


enum Side { WHITE, BLACK };
enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };


class Move
{
protected:
	Side _side;

public:
	Move(Side);
	virtual ~Move() {}

	virtual Side side() const { return _side; }
	virtual operator std::string() const = 0;
};


class NormalMove :
	public Move
{
protected:
	Position _from, _to;

public:
	NormalMove(Side, const Position&, const Position&);

	virtual const Position& from() const { return _from; }
	virtual const Position& to() const { return _to; }
	virtual operator std::string() const override;
};


class CastlingMove :
	public Move
{
public:
	enum Type { KINGSIDE, QUEENSIDE };

private:
	Type _type;
	Position _kingFrom, _kingTo, _rookFrom, _rookTo;

public:
	CastlingMove(Side side, Type type);

	Type type() const { return _type; }
	virtual const Position& kingFrom() const { return _kingFrom; }
	virtual const Position& kingTo() const { return _kingTo; }
	virtual const Position& rookFrom() const { return _rookFrom; }
	virtual const Position& rookTo() const { return _rookTo; }
	virtual operator std::string() const override;
};


class CaptureMove :
	virtual public NormalMove
{
public:
	CaptureMove(Side, const Position&, const Position&);

	virtual const Position& target() const { return _to; }
	virtual operator std::string() const override;
};


class EnPassantCaptureMove :
	public CaptureMove
{
	Position _target;

public:
	EnPassantCaptureMove(Side, const Position&, const Position&);

	virtual const Position& target() const { return _target; }
	virtual operator std::string() const override;
};


class PromotionMove :
	virtual public NormalMove
{
	PieceType _promotion;

public:
	PromotionMove(Side, const Position&, const Position&, PieceType);

	virtual PieceType promotion() const { return _promotion; }
	virtual operator std::string() const override;
};


class CapturePromotionMove :
	public CaptureMove, public PromotionMove
{
public:
	CapturePromotionMove(Side, const Position&, const Position&, PieceType);
	virtual operator std::string() const override;
};