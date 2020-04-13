#pragma once

#include <unordered_map>
#include <fstream>

#include <iostream>

#include "Parser.h"


class Game
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

public:
	Game(const std::string&);
	Game(std::ifstream&);

private:
	void parse(std::ifstream&);
};

