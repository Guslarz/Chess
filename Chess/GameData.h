#pragma once

#include <unordered_map>
#include <fstream>

#include "Parser.h"


class GameData
{
public:
	class Exception : std::exception
	{
		virtual const char* what() const throw() { return "Game exception"; }
	};

	class FileNotFound : Exception 
	{
		virtual const char* what() const throw() { return "File not found"; }
	};

	class InvalidFormat : Exception 
	{
		virtual const char* what() const throw() { return "Invalid format"; }
	};

private:
	std::unordered_map<std::string, std::string> _tags;
	std::unique_ptr<std::vector<Move*>> _moves;
	std::vector<Move*>::const_iterator _currentMove;

public:
	GameData(const std::string&);
	GameData(std::ifstream&);

	const Move* nextMove();
	const Move* prevMove();

private:
	void parse(std::ifstream&);
};

