#pragma once

#include <ostream>

namespace chess {
    class BitBoard;
    enum Direction : int;

    enum File {
        FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
    };
    enum Rank {
        RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
    };

    struct Square {
        int sq;

        constexpr Square() : sq(-1) {}
        Square(const std::string& sq);
        constexpr explicit Square(int sq) : sq(sq) {}
        Square(Rank rank, File file);

        BitBoard as_bb() const;

        std::string str() const;

        bool is_none() const;

        File file() const;
        Rank rank() const;

        Square operator+(const Direction& dir) const;
        bool operator==(const Square& other) const;

        Square& operator+=(const Direction& dir);
    };

    std::ostream& operator<<(std::ostream& os, const Square sq);

    constexpr auto NO_SQUARE = Square(-1);

    constexpr auto a1 = Square(0);
    constexpr auto b1 = Square(1);
    constexpr auto c1 = Square(2);
    constexpr auto d1 = Square(3);
    constexpr auto e1 = Square(4);
    constexpr auto f1 = Square(5);
    constexpr auto g1 = Square(6);
    constexpr auto h1 = Square(7);

    constexpr auto a2 = Square(8);
    constexpr auto b2 = Square(9);
    constexpr auto c2 = Square(10);
    constexpr auto d2 = Square(11);
    constexpr auto e2 = Square(12);
    constexpr auto f2 = Square(13);
    constexpr auto g2 = Square(14);
    constexpr auto h2 = Square(15);

    constexpr auto a3 = Square(16);
    constexpr auto b3 = Square(17);
    constexpr auto c3 = Square(18);
    constexpr auto d3 = Square(19);
    constexpr auto e3 = Square(20);
    constexpr auto f3 = Square(21);
    constexpr auto g3 = Square(22);
    constexpr auto h3 = Square(23);

    constexpr auto a4 = Square(24);
    constexpr auto b4 = Square(25);
    constexpr auto c4 = Square(26);
    constexpr auto d4 = Square(27);
    constexpr auto e4 = Square(28);
    constexpr auto f4 = Square(29);
    constexpr auto g4 = Square(30);
    constexpr auto h4 = Square(31);

    constexpr auto a5 = Square(32);
    constexpr auto b5 = Square(33);
    constexpr auto c5 = Square(34);
    constexpr auto d5 = Square(35);
    constexpr auto e5 = Square(36);
    constexpr auto f5 = Square(37);
    constexpr auto g5 = Square(38);
    constexpr auto h5 = Square(39);

    constexpr auto a6 = Square(40);
    constexpr auto b6 = Square(41);
    constexpr auto c6 = Square(42);
    constexpr auto d6 = Square(43);
    constexpr auto e6 = Square(44);
    constexpr auto f6 = Square(45);
    constexpr auto g6 = Square(46);
    constexpr auto h6 = Square(47);

    constexpr auto a7 = Square(48);
    constexpr auto b7 = Square(49);
    constexpr auto c7 = Square(50);
    constexpr auto d7 = Square(51);
    constexpr auto e7 = Square(52);
    constexpr auto f7 = Square(53);
    constexpr auto g7 = Square(54);
    constexpr auto h7 = Square(55);

    constexpr auto a8 = Square(56);
    constexpr auto b8 = Square(57);
    constexpr auto c8 = Square(58);
    constexpr auto d8 = Square(59);
    constexpr auto e8 = Square(60);
    constexpr auto f8 = Square(61);
    constexpr auto g8 = Square(62);
    constexpr auto h8 = Square(63);
}