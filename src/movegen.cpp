#include "movegen.h"

#include <bit>

#include "bitboard.h"
#include "multiarray.h"
#include "square.h"

namespace chess::movegen {
    MultiArray<BitBoard, 64, 64> LINE{};
    MultiArray<BitBoard, 64, 64> LINESEG{};

    // Magic code from https://github.com/nkarve/surge/blob/master/src/tables.cpp
    constexpr int diagonal_of(Square s) { return 7 + s.rank() - s.file(); }
    constexpr int antidiagonal_of(Square s) { return s.rank() + int(s.file()); }

    // Precomputed diagonal masks
    const BitBoard MASK_DIAGONAL[15] = {
    0x80,
    0x8040,
    0x804020,
    0x80402010,
    0x8040201008,
    0x804020100804,
    0x80402010080402,
    0x8040201008040201,
    0x4020100804020100,
    0x2010080402010000,
    0x1008040201000000,
    0x804020100000000,
    0x402010000000000,
    0x201000000000000,
    0x100000000000000,
    };

    // Precomputed anti-diagonal masks
    const BitBoard MASK_ANTI_DIAGONAL[15] = {
    0x1,
    0x102,
    0x10204,
    0x1020408,
    0x102040810,
    0x10204081020,
    0x1020408102040,
    0x102040810204080,
    0x204081020408000,
    0x408102040800000,
    0x810204080000000,
    0x1020408000000000,
    0x2040800000000000,
    0x4080000000000000,
    0x8000000000000000,
    };

    // Reverses a bitboard
    BitBoard reverse(u64 b) {
        b = (b & 0x5555555555555555) << 1 | ((b >> 1) & 0x5555555555555555);
        b = (b & 0x3333333333333333) << 2 | ((b >> 2) & 0x3333333333333333);
        b = (b & 0x0f0f0f0f0f0f0f0f) << 4 | ((b >> 4) & 0x0f0f0f0f0f0f0f0f);
        b = (b & 0x00ff00ff00ff00ff) << 8 | ((b >> 8) & 0x00ff00ff00ff00ff);

        return (b << 48) | ((b & 0xffff0000) << 16) | ((b >> 16) & 0xffff0000) | (b >> 48);
    }
    BitBoard reverse(BitBoard b) {
        return reverse(b.as_u64());
    }

    // Calculates sliding attacks from a given square, on a given axis, taking into
    // account the blocking pieces. This uses the Hyperbola Quintessence Algorithm.
    BitBoard sliding_attacks(Square square, BitBoard occ, BitBoard mask) { return (((mask & occ) - square.as_bb() * 2) ^ reverse(reverse(mask & occ) - reverse(square.as_bb()) * 2)) & mask; }

    // Returns rook attacks from a given square, using the Hyperbola Quintessence Algorithm. Only used to initialize
    // the magic lookup table
    BitBoard get_rook_attacks_for_init(Square square, BitBoard occ) { return sliding_attacks(square, occ, MASK_FILE[square.file()]) | sliding_attacks(square, occ, MASK_RANK[square.rank()]); }

    BitBoard ROOK_ATTACK_MASKS[64];
    int ROOK_ATTACK_SHIFTS[64];
    BitBoard ROOK_ATTACKS[64][4096];

    const u64 ROOK_MAGICS[64] = {0x0080001020400080, 0x0040001000200040, 0x0080081000200080, 0x0080040800100080, 0x0080020400080080, 0x0080010200040080, 0x0080008001000200, 0x0080002040800100,
                                0x0000800020400080, 0x0000400020005000, 0x0000801000200080, 0x0000800800100080, 0x0000800400080080, 0x0000800200040080, 0x0000800100020080, 0x0000800040800100,
                                0x0000208000400080, 0x0000404000201000, 0x0000808010002000, 0x0000808008001000, 0x0000808004000800, 0x0000808002000400, 0x0000010100020004, 0x0000020000408104,
                                0x0000208080004000, 0x0000200040005000, 0x0000100080200080, 0x0000080080100080, 0x0000040080080080, 0x0000020080040080, 0x0000010080800200, 0x0000800080004100,
                                0x0000204000800080, 0x0000200040401000, 0x0000100080802000, 0x0000080080801000, 0x0000040080800800, 0x0000020080800400, 0x0000020001010004, 0x0000800040800100,
                                0x0000204000808000, 0x0000200040008080, 0x0000100020008080, 0x0000080010008080, 0x0000040008008080, 0x0000020004008080, 0x0000010002008080, 0x0000004081020004,
                                0x0000204000800080, 0x0000200040008080, 0x0000100020008080, 0x0000080010008080, 0x0000040008008080, 0x0000020004008080, 0x0000800100020080, 0x0000800041000080,
                                0x00FFFCDDFCED714A, 0x007FFCDDFCED714A, 0x003FFFCDFFD88096, 0x0000040810002101, 0x0001000204080011, 0x0001000204000801, 0x0001000082000401, 0x0001FFFAABFAD1A2};

    // Initializes the magic lookup table for rooks
    void initialize_rook_attacks() {
        BitBoard edges, subset, index;

        for (int i = 0; i < 64; i++) {
            const Square sq{i};
            edges                  = ((MASK_RANK[FILE_A] | MASK_RANK[FILE_H]) & ~MASK_RANK[sq.rank()]) | ((MASK_FILE[FILE_A] | MASK_FILE[FILE_H]) & ~MASK_FILE[sq.file()]);
            ROOK_ATTACK_MASKS[sq.sq]  = (MASK_RANK[sq.rank()] ^ MASK_FILE[sq.file()]) & ~edges.as_u64();
            ROOK_ATTACK_SHIFTS[sq.sq] = 64 - std::popcount(ROOK_ATTACK_MASKS[sq.sq].as_u64());

            subset = 0;
            do {
                index                   = subset;
                index                   = index.as_u64() * ROOK_MAGICS[sq.sq];
                index                   = index.as_u64() >> ROOK_ATTACK_SHIFTS[sq.sq];
                ROOK_ATTACKS[sq.sq][index.as_u64()] = get_rook_attacks_for_init(sq, subset);
                subset                  = (subset - ROOK_ATTACK_MASKS[sq.sq]) & ROOK_ATTACK_MASKS[sq.sq];
            } while (subset);
        }
    }

    // Returns the attacks bitboard for a rook at a given square, using the magic lookup table
    BitBoard get_rook_attacks(Square sq, BitBoard occ) { return ROOK_ATTACKS[sq.sq][((occ & ROOK_ATTACK_MASKS[sq.sq]).as_u64() * ROOK_MAGICS[sq.sq]) >> ROOK_ATTACK_SHIFTS[sq.sq]]; }

    // Returns the 'x-ray attacks' for a rook at a given square. X-ray attacks cover squares that are not immediately
    // accessible by the rook, but become available when the immediate blockers are removed from the board
    BitBoard get_xray_rook_attacks(Square sq, BitBoard occ, BitBoard blockers) {
        BitBoard attacks = get_rook_attacks(sq, occ);
        blockers &= attacks;
        return attacks ^ get_rook_attacks(sq, occ ^ blockers);
    }

    // Returns bishop attacks from a given square, using the Hyperbola Quintessence Algorithm. Only used to initialize
    // the magic lookup table
    BitBoard get_bishop_attacks_for_init(Square sq, BitBoard occ) {
        return sliding_attacks(sq, occ, MASK_DIAGONAL[diagonal_of(sq)]) | sliding_attacks(sq, occ, MASK_ANTI_DIAGONAL[antidiagonal_of(sq)]);
    }

    u64 BISHOP_ATTACK_MASKS[64];
    int BISHOP_ATTACK_SHIFTS[64];
    u64 BISHOP_ATTACKS[64][512];

    const u64 BISHOP_MAGICS[64] = {0x0002020202020200, 0x0002020202020000, 0x0004010202000000, 0x0004040080000000, 0x0001104000000000, 0x0000821040000000, 0x0000410410400000, 0x0000104104104000,
                                0x0000040404040400, 0x0000020202020200, 0x0000040102020000, 0x0000040400800000, 0x0000011040000000, 0x0000008210400000, 0x0000004104104000, 0x0000002082082000,
                                0x0004000808080800, 0x0002000404040400, 0x0001000202020200, 0x0000800802004000, 0x0000800400A00000, 0x0000200100884000, 0x0000400082082000, 0x0000200041041000,
                                0x0002080010101000, 0x0001040008080800, 0x0000208004010400, 0x0000404004010200, 0x0000840000802000, 0x0000404002011000, 0x0000808001041000, 0x0000404000820800,
                                0x0001041000202000, 0x0000820800101000, 0x0000104400080800, 0x0000020080080080, 0x0000404040040100, 0x0000808100020100, 0x0001010100020800, 0x0000808080010400,
                                0x0000820820004000, 0x0000410410002000, 0x0000082088001000, 0x0000002011000800, 0x0000080100400400, 0x0001010101000200, 0x0002020202000400, 0x0001010101000200,
                                0x0000410410400000, 0x0000208208200000, 0x0000002084100000, 0x0000000020880000, 0x0000001002020000, 0x0000040408020000, 0x0004040404040000, 0x0002020202020000,
                                0x0000104104104000, 0x0000002082082000, 0x0000000020841000, 0x0000000000208800, 0x0000000010020200, 0x0000000404080200, 0x0000040404040400, 0x0002020202020200};

    // Initializes the magic lookup table for bishops
    void initialize_bishop_attacks() {
        u64 edges, subset, index;

        for (int i = 0; i < 64; i++) {
            const Square sq{i};
            edges                    = ((MASK_RANK[FILE_A] | MASK_RANK[FILE_H]) & ~MASK_RANK[sq.rank()]) | ((MASK_FILE[FILE_A] | MASK_FILE[FILE_H]) & ~MASK_FILE[sq.file()]);
            BISHOP_ATTACK_MASKS[sq.sq]  = ((MASK_DIAGONAL[diagonal_of(sq)] ^ MASK_ANTI_DIAGONAL[antidiagonal_of(sq)]) & ~edges).as_u64();
            BISHOP_ATTACK_SHIFTS[sq.sq] = 64 - std::popcount(BISHOP_ATTACK_MASKS[sq.sq]);

            subset = 0;
            do {
                index = subset;
                index = index * BISHOP_MAGICS[sq.sq];
                index = index >> BISHOP_ATTACK_SHIFTS[sq.sq];
                BISHOP_ATTACKS[sq.sq][index] = get_bishop_attacks_for_init(sq, subset).as_u64();
                subset = (subset - BISHOP_ATTACK_MASKS[sq.sq]) & BISHOP_ATTACK_MASKS[sq.sq];
            } while (subset);
        }
    }

    // Returns the attacks bitboard for a bishop at a given square, using the magic lookup table
    u64 get_bishop_attacks(Square sq, u64 occ) { return BISHOP_ATTACKS[sq.sq][((occ & BISHOP_ATTACK_MASKS[sq.sq]) * BISHOP_MAGICS[sq.sq]) >> BISHOP_ATTACK_SHIFTS[sq.sq]]; }

    // Returns the 'x-ray attacks' for a bishop at a given square. X-ray attacks cover squares that are not immediately
    // accessible by the rook, but become available when the immediate blockers are removed from the board
    u64 get_xray_bishop_attacks(Square square, u64 occ, u64 blockers) {
        u64 attacks = get_bishop_attacks(square, occ);
        blockers &= attacks;
        return attacks ^ get_bishop_attacks(square, occ ^ blockers);
    }

    u64 SQUARES_BETWEEN_BB[64][64];

    // Initializes the lookup table for the bitboard of squares in between two given squares (0 if the
    // two squares are not aligned)
    void initialize_squares_between() {
        BitBoard sqs;
        for (int i1 = 0; i1 < 64; i1++) {
            const Square sq1{i1};
            for (int i2 = 0; i2 < 64; i2++) {
                const Square sq2{i2};
                sqs = sq1.as_bb() | sq2.as_bb();
                if (sq1.file() == sq2.file() || sq1.rank() == sq2.rank())
                    SQUARES_BETWEEN_BB[sq1.sq][sq2.sq] = (get_rook_attacks_for_init(sq1, sqs) & get_rook_attacks_for_init(sq2, sqs)).as_u64();
                else if (diagonal_of(sq1) == diagonal_of(sq2) || antidiagonal_of(sq1) == antidiagonal_of(sq2))
                    SQUARES_BETWEEN_BB[sq1.sq][sq2.sq] = (get_bishop_attacks_for_init(sq1, sqs) & get_bishop_attacks_for_init(sq2, sqs)).as_u64();
            }
        }
    }

    // Initializes the lookup table for the bitboard of all squares along the line of two given squares (0 if the
    // two squares are not aligned)
    void initialize_lines() {
        for (int i1 = 0; i1 < 64; i1++) {
            const Square sq1{i1};
            for (int i2 = 0; i2 < 64; i2++) {
                const Square sq2{i2};
                if (sq1.file() == sq2.file() || sq1.rank() == sq2.rank())
                    LINE[sq1.sq][sq2.sq] = (get_rook_attacks_for_init(sq1, 0) & get_rook_attacks_for_init(sq2, 0)) | sq1.as_bb() | sq2.as_bb();
                else if (diagonal_of(sq1) == diagonal_of(sq2) || antidiagonal_of(sq1) == antidiagonal_of(sq2))
                    LINE[sq1.sq][sq2.sq] = (get_bishop_attacks_for_init(sq1, 0) & get_bishop_attacks_for_init(sq2, 0)) | sq1.as_bb() | sq2.as_bb();
            }
        }

        for (int i1 = 0; i1 < 64; i1++) {
            const Square sq1{i1};
            for (int i2 = 0; i2 < 64; i2++) {
                const Square sq2{i2};
                if (sq1 == sq2) {
                    LINESEG[sq1.sq][sq2.sq] = sq1.as_bb();
                    continue;
                }
                BitBoard blockers = sq1.as_bb() | sq2.as_bb();
                if (sq1.file() == sq2.file() || sq1.rank() == sq2.rank())
                    LINESEG[sq1.sq][sq2.sq] = (get_rook_attacks_for_init(sq1, blockers) & get_rook_attacks_for_init(sq2, blockers)) | sq1.as_bb() | sq2.as_bb();
                else if (diagonal_of(sq1) == diagonal_of(sq2) || antidiagonal_of(sq1) == antidiagonal_of(sq2))
                    LINESEG[sq1.sq][sq2.sq] = (get_bishop_attacks_for_init(sq1, blockers) & get_bishop_attacks_for_init(sq2, blockers)) | sq1.as_bb() | sq2.as_bb();
            }
        }
    }

    // Initializes lookup tables for rook moves, bishop moves, in-between squares, aligned squares and pseudolegal moves
    void initialize_movegen_databases() {
        initialize_rook_attacks();
        initialize_bishop_attacks();
        initialize_squares_between();
        initialize_lines();
    }
}