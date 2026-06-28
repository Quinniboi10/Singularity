#include "move.h"

#include "assert.h"
#include "square.h"
#include "board.h"

namespace chess {
    Move::Move(Square startSquare, Square endSquare, MoveType flags) {
        move = startSquare.sq | flags;
        move |= endSquare.sq << 6;
    }

    Move::Move(Square startSquare, Square endSquare, PieceType promo) {
        move = startSquare.sq | PROMOTION;
        move |= endSquare.sq << 6;
        move |= (promo - 1) << 12;
    }

    Move::Move(const Board& board, const std::string& s) {
        const Square from(s.substr(0, 2));
        Square to(s.substr(2, 2));

        MoveType flag = STANDARD_MOVE;

        // Move must be promotion
        if (s.size() > 4) {
            switch (s.at(4)) {
                case 'q':
                    *this = Move(from, to, QUEEN);
                    return;
                case 'r':
                    *this = Move(from, to, ROOK);
                    return;
                case 'b':
                    *this = Move(from, to, BISHOP);
                    return;
                default:
                    *this = Move(from, to, KNIGHT);
                    return;
            }
        }

        // Castling
        if ((from == e1 && to == g1 && board.can_castle(WHITE, KINGSIDE)) || (from == e1 && to == c1 && board.can_castle(WHITE, QUEENSIDE))
            || (from == e8 && to == g8 && board.can_castle(BLACK, KINGSIDE)) || (from == e8 && to == c8 && board.can_castle(BLACK, QUEENSIDE))) {
            to    = board.castle_sq(board.stm, to.sq > from.sq ? KINGSIDE : QUEENSIDE);
            *this = Move(from, to, CASTLE);
            return;
        }

        if (to == board.ep_square && board.pieces(board.stm, PAWN).read_sq(from))
            flag = EN_PASSANT;

        *this = Move(from, to, flag);
    }

    Move Move::null() {
        return Move(a1, a1, STANDARD_MOVE);
    }

    Square Move::from() const {
        return Square(this->move & 0b111111);
    }

    Square Move::to() const {
        return Square((this->move >> 6) & 0b111111);
    }

    PieceType Move::promo() const {
        traced_assert(this->type() == PROMOTION);
        return PieceType(((move >> 12) & 0b11) + 1);
    }

    MoveType Move::type() const {
        return static_cast<MoveType>(this->move & 0xC000);
    }

    std::string Move::str() const {
        const MoveType mt = this->type();

        const Square from = this->from();
        const Square to   = this->to();

        if (mt == STANDARD_MOVE || mt == EN_PASSANT)
            return from.str() + to.str();
        if (mt == CASTLE) {
            const bool is_kingside = to.sq > from.sq;

            return from.str() + (to + (is_kingside ? WEST : EAST_EAST)).str();
        }
        if (mt == PROMOTION) {
            switch (this->promo()) {
                case KNIGHT:
                    return from.str() + to.str() + "n";
                case BISHOP:
                    return from.str() + to.str() + "b";
                case ROOK:
                    return from.str() + to.str() + "r";
                default:
                    return from.str() + to.str() + "q";
            }
        }

        return "BAD_MOVE";
    }


    void MoveList::add(const Move m) {
        this->moves[this->length] = m;
        this->length++;
    }

    void MoveList::add(Square startSquare, Square endSquare, MoveType flags) {
        this->add(Move(startSquare, endSquare, flags));
    }

    void MoveList::add(Square startSquare, Square endSquare, PieceType promo) {
        this->add(Move(startSquare, endSquare, promo));
    }

    usize MoveList::size() const {
        return this->length;
    }
}