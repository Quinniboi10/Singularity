#include <string>
#include <array>

#include "numbers.h"

namespace chess {
    struct Square;

    class BitBoard {
        u64 data;

        public:
        BitBoard(u64 data);

        BitBoard lshift(usize shift) const;
        BitBoard rshift(usize shift) const;

        bool has_data() const;
        bool read_bit(usize idx) const;

        Square get_lsb() const;

        std::string str() const;

        void enable(Square sq);
        void disable(Square sq);

        BitBoard operator|(const BitBoard& other) {
            return BitBoard(this->data | other.data);
        }
    };
}