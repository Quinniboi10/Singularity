#include "bitboard.h"

#include <sstream>
#include <bit>

#include "assert.h"
#include "square.h"
#include "board.h"

namespace chess {
    Direction mirror(const Direction dir) {
        switch (dir) {
            case NORTH:
                return SOUTH;
            case NORTH_EAST:
                return SOUTH_EAST;
            case EAST:
                return EAST;
            case SOUTH_EAST:
                return NORTH_EAST;
            case SOUTH:
                return NORTH;
            case SOUTH_WEST:
                return NORTH_WEST;
            case WEST:
                return WEST;
            case NORTH_WEST:
                return SOUTH_WEST;

            case NORTH_NORTH:
                return SOUTH_SOUTH;
            case EAST_EAST:
                return EAST_EAST;
            case SOUTH_SOUTH:
                return NORTH_NORTH;
            case WEST_WEST:
                return WEST_WEST;
        }
    }

    BitBoard BitBoard::all() {
        return BitBoard(static_cast<u64>(-1));
    }

    BitBoard::BitBoard(const u64 data) {
        this->data = data;
    }

    Direction BitBoard::make_relative(Color c, Direction shift) {
        if (c == WHITE)
            return shift;
        else
            return mirror(shift);
    }

    BitBoard BitBoard::lshift(const usize shift) const {
        traced_assert(shift < 64);
        return BitBoard(this->data << shift);
    }
    BitBoard BitBoard::rshift(const usize shift) const {
        traced_assert(shift < 64);
        return BitBoard(this->data >> shift);
    }
    BitBoard BitBoard::shift(const Direction shift) const {
        if (shift > 0)
            return this->lshift(shift);
        else
            return this->rshift(-shift);
    }

    bool BitBoard::has_data() const {
        return this->data != 0;
    }

    bool BitBoard::read_sq(const Square sq) const {
        return this->data & (1ULL << sq.sq);
    }

    Square BitBoard::get_lsb() const {
        traced_assert(this->data > 0);
        return Square(std::countr_zero(this->data));
    }

    Square BitBoard::pop_lsb() {
        const Square sq = this->get_lsb();
        this->data &= this->data - 1;
        return sq;
    }

    int BitBoard::popcount() const {
        return std::popcount(this->data);
    }

    std::string BitBoard::str() const {
        std::stringstream os{};
        os << "\u250c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n";
        for (int row = 7; row >= 0; row--) {
            os << "\u2502 ";
            for (usize col = 0; col < 8; col++) {
                const Square sq = Square(row * 8 + col);
                os << (read_sq(sq) ? "\u2588" : " ") << " ";
            }
            os << "\u2502 " << row + 1 << "\n";
        }
        os << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n";
        os << "  a b c d e f g h\n";
        return os.str();
    }

    void BitBoard::enable(const Square sq) {
        traced_assert(read_sq(sq) == false);
        this->data |= 1ULL << sq.sq;
    }

    void BitBoard::disable(const Square sq) {
        traced_assert(read_sq(sq) == true);
        this->data &= ~(1ULL << sq.sq);
    }

    u64 BitBoard::as_u64() const {
        return this->data;
    }
}