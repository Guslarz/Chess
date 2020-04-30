#include "Board.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Board::Board(const ShaderProgram *shaderProgram, const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &M) :
	_shaderProgram(shaderProgram), _P(P), _V(V), _M(M), _model(Model::rect)
{
	for (int i = 0; i < 16; ++i) {
		addPiece(new Piece(Model::cube, Position(i % 8, i / 8)));
		addPiece(new Piece(Model::cube, Position(i % 8, 7 - i / 8)));
	}
}


Board::~Board()
{
	for (auto piece : _pieces)
		delete piece;
	for (auto piece : _captured)
		delete piece;
}


void Board::render() const
{
	glm::mat4 PVM = _P * _V * glm::scale(_M, glm::vec3(4.0f, 1.0f, 4.0f));
	glUniformMatrix4fv(_shaderProgram->getUniform("PVM"), 1, GL_FALSE, glm::value_ptr(PVM));
	glUniform4f(_shaderProgram->getUniform("color"), .0f, .0f, .0f, 1.0f);
	_model->render();

	glm::mat4 pieceM = glm::translate(glm::scale(_M, glm::vec3(-1.0f, 1.0f, 1.0f)), glm::vec3(-3.5f, 1.0f, -3.5f));
	glUniform4f(_shaderProgram->getUniform("color"), 1.0f, .0f, .0f, 1.0f);
	for (auto piece : _pieces)
		piece->render(_shaderProgram, _P, _V, pieceM);
}


void Board::applyMove(const Move *move)
{
	if (auto *captureMove = dynamic_cast<const CaptureMove*>(move)) {
		capturePieceAt(captureMove->target());
	}
	if (auto *normalMove = dynamic_cast<const NormalMove*>(move)) {
		movePiece(normalMove->from(), normalMove->to());
	}
	if (auto *promotionMove = dynamic_cast<const PromotionMove*>(move)) {
		//promote
	}
	if (auto *castlingMove = dynamic_cast<const CastlingMove*>(move)) {
		movePiece(castlingMove->kingFrom(), castlingMove->kingTo());
		movePiece(castlingMove->rookFrom(), castlingMove->rookTo());
	}
}


void Board::addPiece(Piece *piece)
{
	_pieces.push_back(piece);
	_board[piece->position()] = piece;
}


void Board::capturePieceAt(const Position &position)
{
	Piece *piece = _board[position];
	_board[position] = nullptr;
	auto it = _pieces.cbegin();
	while (*it != piece) ++it;
	_pieces.erase(it);
	_captured.push_back(piece);
}


void Board::movePiece(const Position &from, const Position &to)
{
	Piece *piece = _board[from];
	_board[from] = nullptr;
	_board[to] = piece;
	piece->setPosition(to);
}