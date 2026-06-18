#pragma once

#include <string>
#include <array>

#include "bitboard.h"
#include "square.h"

namespace chess {
    enum Color {
        BLACK,
        WHITE
    };

    enum PieceType {
        PAWN,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
        KING,
        NO_PIECE_TYPE
    };

    enum CastlingSide {
        KINGSIDE,
        QUEENSIDE
    };

    enum Direction : int {
        NORTH = 8,
        NORTH_EAST = 9,
        EAST = 1,
        SOUTH_EAST = -7,
        SOUTH = -8,
        SOUTH_WEST = -9,
        WEST = -1,
        NORTH_WEST = 7,
        NORTH_NORTH = 16,
        SOUTH_SOUTH = -16
    };

    class Board {
        Color stm;

        std::array<BitBoard, 6> piece_bb;
        std::array<BitBoard, 2> color_bb;

        std::array<Square, 4> castling_rights;

        Square ep_square;

        void clear_sq(Square sq, Color c, PieceType pt);
        void set_sq(Square sq, Color c, PieceType pt);

        Color read_sq_color(const Square sq) const;
        PieceType read_sq(Square sq) const;
        char read_sq_char(Square sq) const;

        static std::tuple<Color, PieceType> parse_piece_char(char c);
        std::tuple<Color, CastlingSide, Square> parse_castling_char(char c) const;

      public:

        Board(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        template<typename... PieceTypes>
        BitBoard pieces(PieceTypes... pts) const {
            return BitBoard::all() & (this->piece_bb[pts] | ...);
        }

        template<typename... PieceTypes>
        BitBoard pieces(Color c, PieceTypes... pts) const {
            return this->color_bb[c] & pieces(pts...);
        }

        BitBoard pieces() const;
        BitBoard pieces(Color c) const;

        void revoke_castle(Color c);
        void revoke_castle(Color c, CastlingSide side);

        bool can_castle(Color c, CastlingSide side) const;

        std::string fen() const;

        friend std::ostream& operator<<(std::ostream& os, const Board& board);
    };
}