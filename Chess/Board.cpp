#include "Board.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"


constexpr float
ANIMATION_DURATION = 1.0f,
MOVE_DURATION_DENOMINATOR = 6.0f,
CURVE_MAX_Y = 2.0f,
CAPTURE_Y = 4.0f;


Board::Board(const ShaderProgram *shaderProgram, const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &M) :
	_time(0.0f), _shaderProgram(shaderProgram), _P(P), _V(V), _M(M), _object(Model::board, Texture::board)
{
	for (unsigned int i = 0; i < 8; ++i) {
		addPiece(Object::piece[WHITE][PAWN], i, 1);
		addPiece(Object::piece[BLACK][PAWN], i, 6);
	}
	addPiece(Object::piece[WHITE][ROOK], 0, 0);
	addPiece(Object::piece[WHITE][ROOK], 7, 0);
	addPiece(Object::piece[BLACK][ROOK], 0, 7);
	addPiece(Object::piece[BLACK][ROOK], 7, 7);

	addPiece(Object::piece[WHITE][KNIGHT], 1, 0);
	addPiece(Object::piece[WHITE][KNIGHT], 6, 0);
	addPiece(Object::piece[BLACK][KNIGHT], 1, 7);
	addPiece(Object::piece[BLACK][KNIGHT], 6, 7);

	addPiece(Object::piece[WHITE][BISHOP], 2, 0);
	addPiece(Object::piece[WHITE][BISHOP], 5, 0);
	addPiece(Object::piece[BLACK][BISHOP], 2, 7);
	addPiece(Object::piece[BLACK][BISHOP], 5, 7);

	addPiece(Object::piece[WHITE][QUEEN], 3, 0);
	addPiece(Object::piece[BLACK][QUEEN], 3, 7);

	addPiece(Object::piece[WHITE][KING], 4, 0);
	addPiece(Object::piece[BLACK][KING], 4, 7);
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
	glm::mat4 M = glm::scale(_M, glm::vec3(4.0f, 1.0f, 4.0f));
	glUniformMatrix4fv(_shaderProgram->getUniform("P"), 1, GL_FALSE, glm::value_ptr(_P));
	glUniformMatrix4fv(_shaderProgram->getUniform("V"), 1, GL_FALSE, glm::value_ptr(_V));
	glUniformMatrix4fv(_shaderProgram->getUniform("M"), 1, GL_FALSE, glm::value_ptr(M));
	_object.render(_shaderProgram);

	glm::mat4 pieceM = glm::translate(_M, glm::vec3(-3.5f, 1.0f, -3.5f));
	for (auto piece : _pieces)
		piece->render(_shaderProgram, pieceM);
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
	for (auto animation : _animations) {
		animation->finish();
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


void Board::addPiece(const Object *object, unsigned int x, unsigned int y)
{
	Piece *piece = new Piece(object, glm::vec3(x, 0.0f, y));
	_board[Position(x, y)] = piece;
	_pieces.push_back(piece);
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
	float x = static_cast<float>(to.x() - from.x()),
		y = static_cast<float>(to.y() - from.y());
	_animations.push_back(new CurveAnimation(0.0f, ANIMATION_DURATION + std::sqrt(x * x + y * y) / MOVE_DURATION_DENOMINATOR, piece, vecFrom, vecTo, CURVE_MAX_Y, [=]() {
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