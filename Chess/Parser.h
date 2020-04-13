#pragma once

#include "Move.h"

#include <vector>
#include <array>
#include <regex>


class Parser
{
	class Piece {
	protected:
		Side _side;
		Position _position;

	public:
		Piece(Side, const Position&);
		virtual ~Piece() {}

		virtual Side side() const { return _side; }
		virtual const Position& position() const { return _position; }

		virtual bool movePossible(const Position&, const std::array<Piece*, 64>&, bool) const = 0;

		virtual void setPosition(const Position &pos) { _position = pos; }

		static PieceType getType(const Piece*);
		static PieceType getType(const std::string&);
	};


	class King :
		public Piece
	{
	public:
		using Piece::Piece;

		virtual bool movePossible(const Position&, const std::array<Piece*, 64>&, bool) const override { return true; }
	};


	class Queen :
		public Piece
	{
	public:
		using Piece::Piece;

		virtual bool movePossible(const Position&, const std::array<Piece*, 64>&, bool) const override;
	};


	class Rook :
		public Piece
	{
	public:
		using Piece::Piece;

		virtual bool movePossible(const Position&, const std::array<Piece*, 64>&, bool) const override;
	};


	class Bishop :
		public Piece
	{
	public:
		using Piece::Piece;

		virtual bool movePossible(const Position&, const std::array<Piece*, 64>&, bool) const override;
	};


	class Knight :
		public Piece
	{
	public:
		using Piece::Piece;

		virtual bool movePossible(const Position&, const std::array<Piece*, 64>&, bool) const override;
	};


	class Pawn :
		public Piece
	{
	public:
		using Piece::Piece;

		virtual bool movePossible(const Position&, const std::array<Piece*, 64>&, bool) const override;
	};


	class Board {
		std::array<std::array<std::vector<Piece*>, 6>, 2> _pieces;
		std::array<Piece*, 64> _board;
		std::vector<Piece*> _captured;

	public:
		Board();
		~Board();

		void makeMove(const Move*);
		void undoMove(const Move*);

		const Piece* king(Side) const;
		const Piece* at(const Position&) const;

		bool isCheck(Side) const;

		const std::vector<Piece*>& pieces(Side, PieceType) const;
		const std::array<Piece*, 64>& board() const { return _board; }

	private:
		void addPiece(Side, PieceType, const Position&);
		void movePiece(const Position&, const Position&);
		void capturePieceAt(const Position&);
		void restoreCapturedAt(const Position&);
	};

public:
	class Exception : std::exception
	{
		virtual const char* what() const throw() { return "Parser exception"; }
	};


	class InvalidMove : Exception
	{
		virtual const char* what() const throw() { return "Invalid move exception"; }
	};

private:
	std::unique_ptr<std::vector<Move*>> _moves;
	Board _board;

	static const std::regex moveRegex;

public:
	Parser();
	~Parser();

	void addMove(const std::string&, const std::string&);
	void addMove(const std::string&, Side);

	std::unique_ptr<std::vector<Move*>>& moves() { return _moves; }

	operator std::string() const;

private:
	Move* parseMove(const std::string&, Side);
};
