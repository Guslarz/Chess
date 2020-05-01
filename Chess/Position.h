#pragma once

#include <string>

#include <glm/glm.hpp>


class Position
{
	int _x, _y;

public:
	Position();
	Position(size_t);
	Position(int, int);
	Position(const std::string&);

	operator size_t() const { return _x * 8 + _y; }
	operator std::string() const;
	operator glm::vec3() const { return glm::vec3(static_cast<float>(_x), 0.0f, static_cast<float>(_y)); }

	int x() const { return _x; }
	int y() const { return _y; }

	Position operator+(const Position&) const;
	Position& operator+=(const Position&);

	bool operator==(const Position&) const;
	bool isBoardPosition() const;
};
