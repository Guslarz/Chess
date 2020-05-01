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
		int x = i % 8, y1 = i / 8, y2 = 7 - y1;
		Piece *piece = new Piece(Model::cube, glm::vec3(x, 0.0f, y1));
		_board[Position(x, y1)] = piece;
		_pieces.push_back(piece);
		piece = new Piece(Model::cube, glm::vec3(x, 0.0f, y2));
		_board[Position(x, y2)] = piece;
		_pieces.push_back(piece);
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


void Board::applyAnimations()
{
	for (auto animation : _animations)
		animation->apply(_time);
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
		makeCastlingMove(castlingMove->kingFrom(), castlingMove->kingTo(), castlingMove->rookFrom(), castlingMove->rookTo());
	}
}


void Board::finishAnimations()
{
	for (auto animation : _animations)
		delete animation;
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


void Board::capturePieceAt(const Position &position)
{
	Piece *piece = _board[position];
	_board[position] = nullptr;
	glm::vec3 from = position,
		to = from;
	to.y = CAPTURE_Y;
	_animations.push_back(new StraightAnimation(0.0f, ANIMATION_DURATION, piece, from, to, [=]() {
		auto it = _pieces.cbegin();
		while (*it != piece) ++it;
		_pieces.erase(it);
		_captured.push_back(piece);
	}));
}


void Board::movePiece(const Position &from, const Position &to)
{
	Piece *piece = _board[from];
	_board[from] = nullptr;
	_board[to] = piece;
	glm::vec3 vecFrom = from,
		vecTo = to;
	_animations.push_back(new CurveAnimation(0.0f, ANIMATION_DURATION, piece, vecFrom, vecTo, CURVE_MAX_Y, [=]() {
		piece->setPosition(vecTo);
	}));
}


void Board::makeCastlingMove(const Position &kingFrom, const Position &kingTo, const Position &rookFrom, const Position &rookTo)
{
	Piece *king = _board[kingFrom],
		*rook = _board[rookFrom];
	_board[kingFrom] = _board[rookFrom] = nullptr;
	_board[kingTo] = king;
	_board[rookTo] = rook;

	_animations.push_back(new CurveAnimation(0.0f, ANIMATION_DURATION, king, kingFrom, kingTo, CURVE_MAX_Y, [=]() {
		king->setPosition(kingTo);
	}));
	_animations.push_back(new StraightAnimation(0.0f, ANIMATION_DURATION, rook,  rookFrom, rookTo, [=]() {
		rook->setPosition(rookTo);
	}));
}