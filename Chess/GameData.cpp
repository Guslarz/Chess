#include "GameData.h"

//#include <iostream>


GameData::GameData(const std::string &filename) 
{
	std::ifstream in(filename);
	parse(in);
	in.close();
}


GameData::GameData(std::ifstream &in)
{
	parse(in);
}


const Move* GameData::nextMove()
{
	auto last = _moves->cend();
	if (_currentMove == last)
		return nullptr;
	return *_currentMove++;
}


const Move* GameData::prevMove()
{
	auto first = _moves->cbegin();
	if (_currentMove == first)
		return nullptr;
	return *--_currentMove;
}


void GameData::parse(std::ifstream &in)
{
	if (!in.is_open())
		throw FileNotFound();

	std::regex tagRegex(
		"^\\["
		"([^ ]*) "
		"\"(.*)\""
		"\\]$"
	), moveRegex(
		"^(\\d+)\\. "
		"([^ ]*) "
		"([^ ]*)"
		"(?: (.*))?"
	);
	std::string line;
	std::smatch result;
	const std::array<std::string, 7> sevenTagRoster({
		"Event", "Site", "Date", "Round",
		"White", "Black", "Result" });

	for (auto &tagname : sevenTagRoster) {
		getline(in, line);
		if (std::regex_match(line, result, tagRegex) && result[1] == tagname)
			_tags[tagname] = result[2];
		else throw InvalidFormat();
	}

	while (getline(in, line) && !line.empty()) {
		if (std::regex_match(line, result, tagRegex))
			_tags[result[1]] = result[2];
		else throw InvalidFormat();
	}

	Parser parser;
	std::string score = _tags.at("Result");
	//std::cout << std::string(parser) << "\n" << std::endl;
	std::string num, white, black;
	in.get();
	while (in >> num >> white >> black) {
		try {
			if (black == score)
				parser.addMove(white, WHITE);
			else parser.addMove(white, black);
		}
		catch (Parser::InvalidMove&) {
			throw InvalidFormat();
		}
		//std::cout << white << " " << black << "\n" << std::string(parser) << "\n" << std::endl;;
	}

	_moves.swap(parser.moves());
	_currentMove = _moves->cbegin();
}