#include "square.h"

#include "bitboard.h"

namespace chess {
    BitBoard Square::as_bb() const {
        return BitBoard(1ULL << this->sq);
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