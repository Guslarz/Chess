#include "Position.h"


Position::Position() :
	_x(0), _y(0)
{}


Position::Position(size_t value) :
	_x(value / 8), _y(value % 8)
{}


Position::Position(int x, int y) :
	_x(x), _y(y)
{}


Position::Position(const std::string &position) :
	Position(position[0] - 'a', position[1] - '1')
{}


Position::operator std::string() const
{
	return std::string({ static_cast<char>(_x + 'a'), static_cast<char>(_y + '1') });
}


Position Position::operator+(const Position &other) const
{
	return Position(_x + other._x, _y + other._y);
}


Position& Position::operator+=(const Position &other)
{
	_x += other._x;
	_y += other._y;
	return *this;
}


bool Position::operator==(const Position &other) const
{
	return _x == other._x && _y == other._y;
}


bool Position::isBoardPosition() const
{
	return _x >= 0 && _x < 8 &&
		_y >= 0 && _y < 8;
}