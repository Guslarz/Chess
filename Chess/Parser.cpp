#include "Parser.h"


Parser::Piece::Piece(Side side, const Position &position) :
	_side(side), _position(position)
{}


bool Parser::Queen::movePossible(const Position &position, const std::array<Piece*, 64> &board, bool) const
{
	int x = position.x() - _position.x(),
		y = position.y() - _position.y();

	Position tmp;
	if (x == 0) tmp = Position(0, y > 0 ? 1 : -1);
	else if (y == 0) tmp = Position(x > 0 ? 1 : -1, 0);
	else if (abs(x) == abs(y)) tmp = Position(x > 0 ? 1 : -1, y > 0 ? 1 : -1);
	else return false;
	Position curr = _position + tmp;
	while (curr.isBoardPosition() && board[curr] == nullptr && curr != position) curr += tmp;
	return curr == position;
}


bool Parser::Rook::movePossible(const Position &position, const std::array<Piece*, 64> &board, bool) const
{
	int x = position.x() - _position.x(),
		y = position.y() - _position.y();

	Position tmp;
	if (x == 0) tmp = Position(0, y > 0 ? 1 : -1);
	else if (y == 0) tmp = Position(x > 0 ? 1 : -1, 0);
	else return false;
	Position curr = _position + tmp;
	while (curr.isBoardPosition() && board[curr] == nullptr && curr != position) curr += tmp;
	return curr == position;
}


bool Parser::Bishop::movePossible(const Position &position, const std::array<Piece*, 64> &board, bool) const
{
	int x = position.x() - _position.x(),
		y = position.y() - _position.y();

	Position tmp;
	if (abs(x) == abs(y)) tmp = Position(x > 0 ? 1 : -1, y > 0 ? 1 : -1);
	else return false;
	Position curr = _position + tmp;
	while (curr.isBoardPosition() && board[curr] == nullptr && curr != position) curr += tmp;
	return curr == position;
}


bool Parser::Knight::movePossible(const Position &position, const std::array<Piece*, 64>&, bool) const
{
	int x = abs(position.x() - _position.x()),
		y = abs(position.y() - _position.y());

	return (x == 1 && y == 2) || (x == 2 && y == 1);
}


bool Parser::Pawn::movePossible(const Position &position, const std::array<Piece*, 64>&, bool capture) const
{
	int forward, first;
	if (side() == WHITE) {
		forward = 1;
		first = 1;
	}
	else {
		forward = -1;
		first = 6;
	}
	if (capture) return position.y() - _position.y() == forward && abs(position.x() - _position.x()) == 1;
	else return _position.x() == position.x() &&
		(position.y() - _position.y() == forward || (position.y() - _position.y() == 2 * forward && _position.y() == first));
}


PieceType Parser::Piece::getType(const Piece *piece)
{
	if (dynamic_cast<const Pawn*>(piece)) return PAWN;
	if (dynamic_cast<const Knight*>(piece)) return KNIGHT;
	if (dynamic_cast<const Bishop*>(piece)) return BISHOP;
	if (dynamic_cast<const Rook*>(piece)) return ROOK;
	if (dynamic_cast<const Queen*>(piece)) return QUEEN;
	if (dynamic_cast<const King*>(piece)) return KING;
	throw std::invalid_argument("getType Piece");
}


PieceType Parser::Piece::getType(const std::string &str)
{
	if (str.empty()) return PAWN;
	switch (str[0])
	{
	case 'K':
		return KING;
	case 'Q':
		return QUEEN;
	case 'R':
		return ROOK;
	case 'B':
		return BISHOP;
	case 'N':
		return KNIGHT;
	}
	throw std::invalid_argument("getType string");
}


Parser::Board::Board()
{
	int x[] = {
		0, 1, 2, 3, 4, 5, 6, 7,
		0, 1, 2, 3, 4, 5, 6, 7,
		0, 1, 2, 3, 4, 5, 6, 7,
		0, 1, 2, 3, 4, 5, 6, 7
	}, y[] = {
		7, 7, 7, 7, 7, 7, 7, 7,
		6, 6, 6, 6, 6, 6, 6, 6,
		1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0
	};
	Side side[] = {
		BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
		BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
		WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
		WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
	};
	PieceType type[] = {
		ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
		PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
		PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
		ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK
	};
	for (size_t i = 0; i < 32; ++i)
		addPiece(side[i], type[i], Position(x[i], y[i]));
}


Parser::Board::~Board()
{
	for (auto *piece : _board)
		if (piece) delete piece;
	for (auto *piece : _captured)
		delete piece;
}


void Parser::Board::makeMove(const Move *move)
{
	if (auto *captureMove = dynamic_cast<const CaptureMove*>(move)) {
		capturePieceAt(captureMove->target());
	}
	if (auto *normalMove = dynamic_cast<const NormalMove*>(move)) {
		movePiece(normalMove->from(), normalMove->to());
	}
	if (auto *promotionMove = dynamic_cast<const PromotionMove*>(move)) {
		capturePieceAt(promotionMove->to());
		addPiece(move->side(), promotionMove->promotion(), promotionMove->to());
	}
	if (auto *castlingMove = dynamic_cast<const CastlingMove*>(move)) {
		movePiece(castlingMove->kingFrom(), castlingMove->kingTo());
		movePiece(castlingMove->rookFrom(), castlingMove->rookTo());
	}
}


void Parser::Board::undoMove(const Move *move)
{
	if (auto *castlingMove = dynamic_cast<const CastlingMove*>(move)) {
		movePiece(castlingMove->rookTo(), castlingMove->rookFrom());
		movePiece(castlingMove->kingTo(), castlingMove->kingFrom());
	}
	if (auto *promotionMove = dynamic_cast<const PromotionMove*>(move)) {
		Piece *piece = _board[promotionMove->to()];
		auto &vec = _pieces[piece->side()][Piece::getType(piece)];
		auto it = vec.cbegin();
		while (*it != piece) ++it;
		vec.erase(it);
		restoreCapturedAt(promotionMove->to());
	}
	if (auto *normalMove = dynamic_cast<const NormalMove*>(move)) {
		movePiece(normalMove->to(), normalMove->from());
	}
	if (auto *captureMove = dynamic_cast<const CaptureMove*>(move)) {
		restoreCapturedAt(captureMove->target());
	}
}


const Parser::Piece* Parser::Board::king(Side side) const
{
	return _pieces[side][KING].front();
}


const Parser::Piece* Parser::Board::at(const Position &position) const
{
	return _board[position];
}


bool Parser::Board::isCheck(Side side) const
{
	Position kingPosition = _pieces[side][KING].front()->position();
	Side opposite = side == WHITE ? BLACK : WHITE;

	for (size_t i = 1; i < 6; ++i)
		for (auto piece : _pieces[opposite][i])
			if (piece->movePossible(kingPosition, _board, true))
				return true;
	return false;
}


const std::vector<Parser::Piece*>& Parser::Board::pieces(Side side, PieceType type) const
{
	return _pieces[side][type];
}


void Parser::Board::addPiece(Side side, PieceType type, const Position &position)
{
	Piece *piece;
	switch (type) {
	case KING:
		piece = new King(side, position);
		break;
	case QUEEN:
		piece = new Queen(side, position);
		break;
	case ROOK:
		piece = new Rook(side, position);
		break;
	case BISHOP:
		piece = new Bishop(side, position);
		break;
	case KNIGHT:
		piece = new Knight(side, position);
		break;
	case PAWN:
		piece = new Pawn(side, position);
		break;
	}
	_pieces[side][type].push_back(piece);
	_board[position] = piece;
}


void Parser::Board::movePiece(const Position &from, const Position &to)
{
	Piece *piece = _board[from];
	_board[from] = nullptr;
	_board[to] = piece;
	piece->setPosition(to);
}


void Parser::Board::capturePieceAt(const Position &position)
{
	Piece *piece = _board[position];
	_board[position] = nullptr;
	auto &vec = _pieces[piece->side()][Piece::getType(piece)];
	auto it = vec.cbegin();
	while (*it != piece) ++it;
	vec.erase(it);
	_captured.push_back(piece);
}


void Parser::Board::restoreCapturedAt(const Position &position)
{
	Piece *piece = _captured.back();
	_captured.pop_back();
	_board[position] = piece;
	_pieces[piece->side()][Piece::getType(piece)].push_back(piece);
	piece->setPosition(position);
}


const std::regex Parser::moveRegex(
	"^"
	"([KQRBN]?)"
	"([a-h]?)"
	"([1-8]?)"
	"(x?)"
	"([a-h][1-8])"
	"((?:=[KQRBN])?)"
	"([\\+#]?)"
	"$"
);


Parser::Parser() :
	_moves(std::make_unique<std::vector<Move*>>())
{}


Parser::~Parser()
{
	if (_moves) {
		for (auto move : *_moves)
			delete move;
	}
}


void Parser::addMove(const std::string &white, const std::string &black)
{
	addMove(white, WHITE);
	addMove(black, BLACK);
}


void Parser::addMove(const std::string &move_str, Side side)
{
	Move *move = parseMove(move_str, side);
	_board.makeMove(move);
	_moves->push_back(move);
}


Parser::operator std::string() const
{
	int x, y;
	std::string str;

	str = "   a b c d e f g h   \n  +-+-+-+-+-+-+-+-+  \n";

	for (y = 7; y >= 0; --y) {
		str += y + '1';
		str += " |";
		for (x = 0; x < 8; ++x) {
			auto *tmp = _board.at(Position(x, y));
			char sign;
			if (tmp == nullptr) sign = ' ';
			else {
				PieceType type = Piece::getType(tmp);
				if (type == KING) sign = 'K';
				else if (type == QUEEN) sign = 'Q';
				else if (type == ROOK) sign = 'R';
				else if (type == BISHOP) sign = 'B';
				else if (type == KNIGHT) sign = 'N';
				else sign = 'P';
			}
			str += sign;
			str += '|';
		}
		str += ' ';
		str += y + '1';
		str += "\n  +-+-+-+-+-+-+-+-+  \n";
	}

	str += "   a b c d e f g h   \n";
	return str;
}


Move* Parser::parseMove(const std::string &move, Side side)
{
	if (move == "O-O")
		return new CastlingMove(side, CastlingMove::KINGSIDE);
	if (move == "O-O-O")
		return new CastlingMove(side, CastlingMove::QUEENSIDE);

	std::smatch match;
	if (std::regex_match(move, match, moveRegex)) {
		const Piece *piece = nullptr;

		Position to(match[5]);
		bool capture = !match[4].str().empty();
		bool promotion = !match[6].str().empty();
		PieceType type = Piece::getType(match[1].str());

		if (type == KING)
			piece = _board.king(side);
		else if (!(match[2].str().empty() || match[3].str().empty()))
			piece = _board.at(Position(match[2].str()[0], match[3].str()[0]));
		else {
			int x, y;
			x = y = -1;
			if (!match[2].str().empty())
				x = match[2].str()[0] - 'a';
			if (!match[3].str().empty())
				x = match[3].str()[0] - '1';
			auto &pieces = _board.pieces(side, type);
			for (auto &tmp : pieces) {
				if (x != -1 && tmp->position().x() != x)
					continue;
				if (y != -1 && tmp->position().y() != y)
					continue;
				if (tmp->movePossible(to, _board.board(), capture)) {
					piece = tmp;

					Move *move;

					if (capture) {
						if (promotion) move = new CapturePromotionMove(side, piece->position(), to, Piece::getType(match[6].str()));
						else if (type == PAWN && _board.at(to) == nullptr)
							move = new EnPassantCaptureMove(side, piece->position(), to);
						else move = new CaptureMove(side, piece->position(), to);
					}
					else if (promotion)
						move = new PromotionMove(side, piece->position(), to, Piece::getType(match[6].str()));
					else move = new NormalMove(side, piece->position(), to);

					_board.makeMove(move);
					if (_board.isCheck(side)) {
						_board.undoMove(move);
					}
					else {
						_board.undoMove(move);
						return move;
					}
				}
			}
		}

		if (piece == nullptr)
			throw InvalidMove();

		if (capture) {
			if (promotion) return new CapturePromotionMove(side, piece->position(), to, Piece::getType(match[6].str()));
			else if (type == PAWN && _board.at(to) == nullptr)
				return new EnPassantCaptureMove(side, piece->position(), to);
			else return new CaptureMove(side, piece->position(), to);
		}
		else if (promotion)
			return new PromotionMove(side, piece->position(), to, Piece::getType(match[6].str()));
		else return new NormalMove(side, piece->position(), to);
	}
	else throw InvalidMove();
}