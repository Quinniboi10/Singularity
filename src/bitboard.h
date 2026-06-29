#pragma once

#include <compare>
#include <string>
#include <array>

#include "numbers.h"

namespace chess {
    struct Square;
    enum Direction : i8;
    enum Color : bool;

    class BitBoard {
        u64 data;

       public:
        static BitBoard all();

        constexpr BitBoard(u64 data = 0) : data(data) {};

        static Direction make_relative(Color c, Direction shift);

        BitBoard lshift(usize shift) const;
        BitBoard rshift(usize shift) const;
        BitBoard shift(Direction shift) const;

        bool has_data() const;
        bool read_sq(Square idx) const;

        Square get_lsb() const;
        Square pop_lsb();

        int popcount() const;

        std::string str() const;

        void enable(Square sq);
        void disable(Square sq);

        u64 as_u64() const;

        std::strong_ordering operator<=>(const BitBoard& other) const {
            return this->data <=> other.data;
        }

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

        BitBoard& operator|=(const BitBoard& other) {
            this->data |= other.data;
            return *this;
        }
        BitBoard& operator&=(const BitBoard& other) {
            this->data &= other.data;
            return *this;
        }
        BitBoard& operator^=(const BitBoard& other) {
            this->data ^= other.data;
            return *this;
        }

        explicit operator bool() const {
            return static_cast<bool>(this->data);
        }
    };
}