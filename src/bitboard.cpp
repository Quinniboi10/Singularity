#include "bitboard.h"

#include <sstream>
#include <bit>

#include "assert.h"
#include "square.h"

namespace chess {
    BitBoard BitBoard::all() {
        return BitBoard(static_cast<u64>(-1));
    }

    BitBoard::BitBoard(const u64 data) {
        this->data = data;
    }

    BitBoard BitBoard::lshift(const usize shift) const {
        traced_assert(shift < 64);
        return BitBoard(this->data << shift);
    }
    BitBoard BitBoard::rshift(const usize shift) const {
        traced_assert(shift < 64);
        return BitBoard(this->data >> shift);
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
}