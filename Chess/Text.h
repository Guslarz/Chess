#pragma once

#include <vector>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <array>


constexpr size_t ASCII_LOADED = 128;


class Text
{
	struct Char
	{
		unsigned int tex;
		glm::ivec2 size, bearing;
		FT_Pos advance;
	};

	struct RenderedChar
	{
		unsigned int tex;
		glm::vec2 pos, size;
	};

	std::vector<RenderedChar> _characters;

	static FT_Library _library;
	static FT_Face _face;
	static std::array<Char, ASCII_LOADED> _chars;

public:
	Text(const std::vector<std::string>&, float, float, float, float);
	~Text();

	void render() const;

	static void test();

	static void loadTexts();
	static void deleteTexts();
};

