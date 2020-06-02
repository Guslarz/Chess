#include "Text.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"

#include "Texture.h"


FT_Library Text::_library;
FT_Face Text::_face;
std::array<Text::Char, ASCII_LOADED> Text::_chars;


constexpr float
INITIAL_CHAR_HEIGHT = 64.0f,
LINE_HEIGHT = 1.15f;


Text::Text(const std::vector<std::string> &lines, float x, float y, float width, float height)
{
	int maxLineAdvance = 0;
	std::vector<unsigned int> lineAdvance;
	for (auto &line : lines) {
		unsigned int currLineAdvance = 0;
		for (auto &c : line) {
			Char character = _chars[static_cast<size_t>(c)];
			currLineAdvance += character.advance;
		}
		if (currLineAdvance > maxLineAdvance)
			maxLineAdvance = currLineAdvance;
		lineAdvance.push_back(currLineAdvance);
	}
	float sx = width / static_cast<float>(maxLineAdvance),
		sy = height / (lines.size() * INITIAL_CHAR_HEIGHT * LINE_HEIGHT);



	float currX = x,
		currY = y + height - INITIAL_CHAR_HEIGHT * LINE_HEIGHT * sy;
	for (size_t i = 0; i < lines.size(); ++i) {
		auto &line = lines[i];
		for (auto &c : line) {
			Char character = _chars[c];

			float xpos = currX + character.bearing.x * sx + (width - lineAdvance[i] * sx) / 2.0f,
				ypos = currY - (character.size.y - character.bearing.y) * sy,
				w = character.size.x * sx,
				h = character.size.y * sy;

			_characters.push_back({
				character.tex,
				glm::vec2(xpos, ypos),
				glm::vec2(w, h)
			});

			currX += character.advance * sx;
		}
		currX = x;
		currY -= INITIAL_CHAR_HEIGHT * LINE_HEIGHT * sy;
	}
}


Text::~Text()
{}


void Text::render() const
{
	static float uvs[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	glUniform1i(ShaderProgram::uiShader->getUniform("texSampler0"), 0);
	glActiveTexture(GL_TEXTURE0);

	for (auto &c : _characters) {
		float vertices[] = {
			c.pos.x, c.pos.y + c.size.y,
			c.pos.x, c.pos.y,
			c.pos.x + c.size.x, c.pos.y,

			c.pos.x, c.pos.y + c.size.y,
			c.pos.x + c.size.x, c.pos.y,
			c.pos.x + c.size.x, c.pos.y + c.size.y
		};

		glBindTexture(GL_TEXTURE_2D, c.tex);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, uvs);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}


void Text::loadTexts()
{
	if (FT_Init_FreeType(&_library))
		printf("FT_Init_FreeType error\n");
	if (FT_New_Face(_library, "fonts/arial.ttf", 0, &_face))
		printf("FT_New_Face error\n");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLuint textures[ASCII_LOADED];
	glGenTextures(ASCII_LOADED, &textures[0]);

	FT_Set_Pixel_Sizes(_face, 0, static_cast<int>(INITIAL_CHAR_HEIGHT));
	for (size_t i = 0; i < ASCII_LOADED; ++i) {
		if (FT_Load_Char(_face, static_cast<FT_ULong>(i), FT_LOAD_RENDER)) {
			printf("FT_Load_Char error\n");
			continue;
		}

		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _face->glyph->bitmap.width, _face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, _face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		_chars[i] = {
			textures[i],
			glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
			glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
			_face->glyph->advance.x >> 6
		};
	}
}


void Text::deleteTexts()
{
	for (auto &c : _chars)
		glDeleteTextures(1, &c.tex);

	FT_Done_Face(_face);
	FT_Done_FreeType(_library);
}


void Text::test()
{

	static float vertices[] = {
		-1.0f, -1.0f,	1.0f, 1.0f,		-1.0f, 1.0f,
		-1.0f, -1.0f,	1.0f, 1.0f,		1.0f, -1.0f
	}, uvs[] = {
		0.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f,
		0.0f, 1.0f,		1.0f, 0.0f,		1.0f, 1.0f
	};
	static GLsizei count = 6;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _chars['A'].tex);
	glUniform1i(ShaderProgram::uiShader->getUniform("texSampler0"), 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, uvs);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}