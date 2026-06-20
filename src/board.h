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
        NORTH_NORTH = 16,
        NORTH_EAST = 9,
        EAST = 1,
        EAST_EAST = 2,
        SOUTH_EAST = -7,
        SOUTH = -8,
        SOUTH_SOUTH = -16,
        SOUTH_WEST = -9,
        WEST = -1,
        WEST_WEST = -2,
        NORTH_WEST = 7
    };

    class Board {
        std::array<BitBoard, 6> piece_bb;
        std::array<BitBoard, 2> color_bb;

        std::array<Square, 4> castling_rights;

        void clear_sq(Square sq, Color c, PieceType pt);
        void set_sq(Square sq, Color c, PieceType pt);

        char read_sq_char(Square sq) const;

        static std::tuple<Color, PieceType> parse_piece_char(char c);
        std::tuple<Color, CastlingSide, Square> parse_castling_char(char c) const;

      public:
      
        Color stm;

        Square ep_square;

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
        
        Color read_sq_color(const Square sq) const;
        PieceType read_sq(Square sq) const;

        void revoke_castle(Color c);
        void revoke_castle(Color c, CastlingSide side);

        bool can_castle(Color c, CastlingSide side) const;
        Square castle_sq(Color c, CastlingSide side) const;

        std::string fen() const;

        friend std::ostream& operator<<(std::ostream& os, const Board& board);
    };
}