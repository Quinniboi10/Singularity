#pragma once

#include <string>
#include <array>

#include "numbers.h"

namespace chess {
    struct Square;

    class BitBoard {
        u64 data;

        public:
        static BitBoard all();

        BitBoard(u64 data = 0);

        BitBoard lshift(usize shift) const;
        BitBoard rshift(usize shift) const;

        bool has_data() const;
        bool read_sq(Square idx) const;

        Square get_lsb() const;

        std::string str() const;

        void enable(Square sq);
        void disable(Square sq);

        u64 as_u64() const;

        BitBoard operator|(const BitBoard& other) const {
            return BitBoard(this->data | other.data);
        }
        BitBoard operator&(const BitBoard& other) const {
            return BitBoard(this->data & other.data);
        }
        BitBoard operator^(const BitBoard& other) const {
            return BitBoard(this->data ^ other.data);
        }
        BitBoard operator~() const {
            return BitBoard(~this->data);
        }
        
        BitBoard operator+(const BitBoard& other) const {
            return BitBoard(this->data + other.data);
        }
        BitBoard operator-(const BitBoard& other) const {
            return BitBoard(this->data - other.data);
        }
        BitBoard operator*(const BitBoard& other) const {
            return BitBoard(this->data * other.data);
        }

        BitBoard& operator&=(const BitBoard& other) {
            this->data &= other.data;
            return *this;
        }

        explicit operator bool() const {
            return static_cast<bool>(this->data);
        }
    };
}