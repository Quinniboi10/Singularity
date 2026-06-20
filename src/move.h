#pragma once

#include "numbers.h"
#include "square.h"
#include "board.h"

#include <array>
#include <string>

namespace chess {
    enum MoveType {
        STANDARD_MOVE = 0, EN_PASSANT = 0x4000, CASTLE = 0x8000, PROMOTION = 0xC000
    };
    
    class Move {
        u16 move;

        public:
        constexpr Move() = default;
        constexpr ~Move() = default;

        Move(Square startSquare, Square endSquare, MoveType flags = STANDARD_MOVE);
        Move(Square startSquare, Square endSquare, PieceType promo);

        static Move null();

        Square from() const;
        Square to() const;

        PieceType promo() const;

        MoveType type() const;

        std::string str() const;
    };

    class MoveList {
        std::array<Move, 256> moves;
        usize length = 0;

        public:
        constexpr MoveList() = default;
        constexpr ~MoveList() = default;

        void add(Move m);
        void add(Square startSquare, Square endSquare, MoveType flags = STANDARD_MOVE);
        void add(Square startSquare, Square endSquare, PieceType promo);

        auto begin() { return moves.begin(); }
        auto end() { return moves.begin() + length; }
        auto begin() const { return moves.begin(); }
        auto end() const { return moves.begin() + length; }
    };
}