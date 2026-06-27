#pragma once

#include <string>
#include <array>

#include "bitboard.h"
#include "square.h"

namespace chess {
    enum Color : int {
        BLACK,
        WHITE
    };

    inline Color operator~(const Color& other) { return static_cast<Color>(other ^ 1); }

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
        QUEENSIDE,
        KINGSIDE
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

    inline Direction operator+(const Direction& lhs, const Direction& rhs) {
        return static_cast<Direction>(static_cast<int>(lhs) + rhs);
    }
    inline Direction operator*(const Direction& lhs, const int& rhs) {
        return static_cast<Direction>(static_cast<int>(lhs) * rhs);
    }

    class Move;

    class Board {
        std::array<BitBoard, 6> piece_bb;
        std::array<BitBoard, 2> color_bb;

        std::array<BitBoard, 2> attacking_bb;
        std::array<BitBoard, 2> pinner_bb;

        std::array<Square, 4> castling_rights;

        void clear_sq(Square sq, Color c, PieceType pt);
        void set_sq(Square sq, Color c, PieceType pt);

        char read_sq_char(Square sq) const;

        static std::tuple<Color, PieceType> parse_piece_char(char c);
        std::tuple<Color, CastlingSide, Square> parse_castling_char(char c) const;

        void update_check_pin_attack();

      public:
      
        Color stm;

        Square ep_square;
        
        BitBoard checkers;
        BitBoard pinned;
        BitBoard check_mask;

        Board(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        Square rel_sq_shift(Square sq, Direction dir) const;
        Direction rel_shift_dir(Direction dir) const;

        template<PieceType pt>
        bool has_piece(const Square sq) const {
            return this->pieces(pt).read_sq(sq);
        }

        template<PieceType pt>
        bool has_piece(const Color c, const Square sq) const {
            return this->pieces(c, pt).read_sq(sq);
        }

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

        BitBoard attacking(Color c) const;

        bool in_check() const;

        bool is_capture(Move m) const;
        
        Color read_sq_color(const Square sq) const;
        PieceType read_sq(Square sq) const;

        void revoke_castle(Color c);
        void revoke_castle(Color c, CastlingSide side);

        bool can_castle(Color c, CastlingSide side) const;
        Square castle_sq(Color c, CastlingSide side) const;

        Board move(Move m) const;

        std::string fen() const;

        friend std::ostream& operator<<(std::ostream& os, const Board& board);
    };
}