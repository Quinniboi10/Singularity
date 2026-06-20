#include "square.h"

#include <sstream>

#include "bitboard.h"

namespace chess {
    Square::Square(const std::string& sq) {
        this->sq = (sq[1] - '1') * 8 + (sq[0] - 'a');
    }

    Square::Square(const Rank rank, const File file) {
        this->sq = rank * 8 + file;
    }

    BitBoard Square::as_bb() const {
        return BitBoard(1ULL << this->sq);
    }

    std::string Square::str() const {
        std::ostringstream os;
        os << *this;
        return os.str();
    }

    bool Square::is_none() const {
        return *this == NO_SQUARE;
    }

    File Square::file() const {
        return static_cast<File>(this->sq & 0b111);
    }

    Rank Square::rank() const {
        return static_cast<Rank>(this->sq >> 3);
    }

    Square Square::operator+(const Direction& dir) const {
        return Square(this->sq + dir);
    }

    bool Square::operator==(const Square& other) const {
        return this->sq == other.sq;
    }
    
    Square& Square::operator+=(const Direction& dir) {
        this->sq += dir;
        return *this;
    }

    std::ostream& operator<<(std::ostream& os, const Square sq) {
        const int idx = sq.sq;
        if (idx == -1)
            return os << "NO_SQUARE";
        else if (idx < 0 || idx >= 64)
            return os << "BAD_SQUARE";
        return os << static_cast<char>('a' + idx % 8) << std::to_string(idx / 8 + 1);
    }
}