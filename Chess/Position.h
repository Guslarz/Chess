#pragma once

#include <string>


class Position
{
	int _x, _y;

public:
	Position();
	Position(size_t);
	Position(int, int);
	Position(const std::string&);

	operator size_t() const { return _x * 8 + _y; }

	int x() const { return _x; }
	int y() const { return _y; }

	Position operator+(const Position&) const;
	Position& operator+=(const Position&);

	bool operator==(const Position&) const;
	bool isBoardPosition() const;
};
