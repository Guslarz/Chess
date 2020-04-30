#include "Board.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


constexpr float
ANIMATION_DURATION = 1.0f,
CURVE_MAX_Y = 4.0f,
CAPTURE_Y = 4.0f;


Board::Board(const ShaderProgram *shaderProgram, const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &M) :
	_time(0.0f), _shaderProgram(shaderProgram), _P(P), _V(V), _M(M), _model(Model::rect)
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
	for (auto animation : _animations)
		animation->render(_shaderProgram, _P, _V, pieceM, _time);
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
	}
	if (auto *castlingMove = dynamic_cast<const CastlingMove*>(move)) {
		movePiece(castlingMove->kingFrom(), castlingMove->kingTo());
		movePiece(castlingMove->rookFrom(), castlingMove->rookTo());
	}
}


void Board::finishAnimations()
{
	for (auto animation : _animations) {
		Piece *piece = animation->target();
		auto &position = piece->position();
		if (position.y() >= 0 && position.y() < 8 && position.x() >= 0 && position.x() < 8)
			_pieces.push_back(piece);
		else _captured.push_back(piece);
		delete animation;
	}
	_animations.erase(_animations.cbegin(), _animations.cend());
	_time = 0.0f;
}


bool Board::finished() const
{
	if (_animations.empty())
		return true;
	for (auto animation : _animations)
		if (!animation->finished(_time))
			return true;
	return false;
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
	piece->setPosition(Position(-1, -1));
	_animations.push_back(new StraightAnimation(0.0f, ANIMATION_DURATION, piece, position.x(), position.x(), 0.0f, CAPTURE_Y, position.y(), position.y()));
}


void Board::movePiece(const Position &from, const Position &to)
{
	Piece *piece = _board[from];
	_board[from] = nullptr;
	_board[to] = piece;
	piece->setPosition(to);
	auto it = _pieces.cbegin();
	while (*it != piece) ++it;
	_pieces.erase(it);
	_animations.push_back(new CurveAnimation(0.0f, ANIMATION_DURATION, piece, from.x(), to.x(), 0.0f, CURVE_MAX_Y, from.y(), to.y()));
}