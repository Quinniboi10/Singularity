#include "board.h"

#include <ostream>
#include <stdexcept>
#include <deque>
#include <tuple>

#include "../external/fmt/fmt/color.h"

#include "square.h"
#include "utils.h"

namespace chess {
    std::tuple<Color, PieceType> Board::parse_piece_char(char c) {
        const bool is_upper = c >= 'A' && c <= 'Z';
        // Convert to lowercase
        if (is_upper)
            c += 'a' - 'A';

        PieceType pt;
        switch (c) {
            case 'p':
                pt = PAWN;
                break;
            case 'n':
                pt = KNIGHT;
                break;
            case 'b':
                pt = BISHOP;
                break;
            case 'r':
                pt = ROOK;
                break;
            case 'q':
                pt = QUEEN;
                break;
            case 'k':
                pt = KING;
                break;
            default:
                throw std::runtime_error("Failed to convert char to piece. Unexpected char: " + std::to_string(c));
        }

        return std::make_tuple(is_upper ? WHITE : BLACK, pt);
    }

    std::tuple<Color, CastlingSide, Square> Board::parse_castling_char(char c) const {
        const bool is_upper = c >= 'A' && c <= 'Z';
        // Convert to lowercase
        if (is_upper)
            c += 'a' - 'A';

        const Color color = is_upper ? WHITE : BLACK;
        const CastlingSide side = c == 'k' ? KINGSIDE : QUEENSIDE;

        const Direction dir = side == KINGSIDE ? EAST : WEST;
        const Square king_sq = this->pieces(color, KING).get_lsb();

        const BitBoard rook_bb = this->pieces(color, ROOK);

        Square rook_sq = king_sq;
        do {
            rook_sq += dir;
        } while (rook_bb.read_sq(rook_sq) == false);

        return std::make_tuple(color, side, rook_sq);
    }

    int castle_idx(const Color c, const CastlingSide side) {
        return 2 * c + side;
    }

    void Board::clear_sq(const Square sq, const Color c, const PieceType pt) {
        this->piece_bb[pt].disable(sq);
        this->color_bb[c].disable(sq);
    }

    void Board::set_sq(const Square sq, const Color c, const PieceType pt) {
        this->piece_bb[pt].enable(sq);
        this->color_bb[c].enable(sq);
    }

    PieceType Board::read_sq(const Square sq) const {
        for (const PieceType pt : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING})
            if (this->pieces(pt).read_sq(sq))
                return pt;
        return NO_PIECE_TYPE;
    }

    Color Board::read_sq_color(const Square sq) const {
        return this->pieces(WHITE).read_sq(sq) ? WHITE : BLACK;
    }

    char Board::read_sq_char(const Square sq) const {
        const bool is_white = this->read_sq_color(sq) == WHITE;
        const PieceType pt = this->read_sq(sq);
        switch (pt) {
            case PAWN:
                return is_white ? 'P' : 'p';
            case KNIGHT:
                return is_white ? 'N' : 'n';
            case BISHOP:
                return is_white ? 'B' : 'b';
            case ROOK:
                return is_white ? 'R' : 'r';
            case QUEEN:
                return is_white ? 'Q' : 'q';
            case KING:
                return is_white ? 'K' : 'k';
            default:
                return ' ';
        }
    }

    Board::Board(const std::string& fen) {
        std::vector<std::string> tokens = split(fen, ' ');

        // Position parsing
        std::deque<char> pos(tokens[0].begin(), tokens[0].end());

        for (int rank = 7; rank >= 0; rank--) {
            for (int file = 0; file < 8; file++) {
                const Square sq = Square(static_cast<Rank>(rank), static_cast<File>(file));
                const char c = pos.front();
                pos.pop_front();

                // Parse the case of skipping
                int as_num = c - '0';
                if (as_num > 0 && as_num <= 9)
                    file += as_num - 1;
                // Assume it's a parsable char, otherwise it's the user's fault
                else {
                    const auto [color, pt] = parse_piece_char(c);
                    this->set_sq(sq, color, pt);
                }
            }
            // Eat the '/' delim char
            if (rank > 0)
                pos.pop_front();
        }

        // STM parsing
        this->stm = tokens[1][0] == 'w' ? WHITE : BLACK;

        // Castling
        std::string s = tokens[2];
        this->castling_rights = {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE};
        if (s[0] != '-') {
            while (s[0] != '\0') {
                const auto [color, side, rook_sq] = parse_castling_char(s[0]);

                this->castling_rights[castle_idx(color, side)] = rook_sq;

                s = s.substr(1, s.length() - 1);
            }
        }

        // EP
        if (tokens[3][0] == '-')
            this->ep_square = NO_SQUARE;
        else
            this->ep_square = Square(tokens[3]);
    }

    BitBoard Board::pieces() const {
        return this->pieces(WHITE) | this->pieces(BLACK);
    }

    BitBoard Board::pieces(const Color c) const {
        return this->color_bb[c];
    }

    bool Board::can_castle(const Color c, const CastlingSide side) const {
        return this->castling_rights[castle_idx(c, side)] != NO_SQUARE;
    }

    void Board::revoke_castle(const Color c) {
        this->revoke_castle(c, KINGSIDE);
        this->revoke_castle(c, QUEENSIDE);
    }

    void Board::revoke_castle(const Color c, const CastlingSide side) {
        this->castling_rights[castle_idx(c, side)] = NO_SQUARE;
    }

    Square Board::castle_sq(const Color c, const CastlingSide side) const {
        return this->castling_rights[castle_idx(c, side)];
    }

    std::string Board::fen() const {
        std::string res = "";

        // Position
        for (int rank = 7; rank >= 0;rank--) {
            int skip = 0;
            for (int file = 0; file < 8; file++) {
                const Square sq(rank * 8 + file);
                const char piece = this->read_sq_char(sq);
                if (piece == ' ')
                    skip++;
                else {
                    if (skip > 0) {
                        res += std::to_string(skip);
                        skip = 0;
                    }
                    res += piece;
                }
            }
            if (skip > 0)
                res += std::to_string(skip);
            if (rank > 0)
                res += "/";
        }

        // Side to move
        if (this->stm == WHITE)
            res += " w ";
        else
            res += " b ";

        // Castling
        std::string castling = "";
        if (this->can_castle(WHITE, KINGSIDE))
            castling += "K";
        if (this->can_castle(WHITE, QUEENSIDE))
            castling += "Q";
        if (this->can_castle(BLACK, KINGSIDE))
            castling += "k";
        if (this->can_castle(BLACK, QUEENSIDE))
            castling += "q";
        
        if (castling.empty())
            res += "- ";
        else
            res += castling + " ";

        // En passant
        if (this->ep_square.is_none())
            res += "-";
        else
            res += this->ep_square.str();

        return res;
    }

    std::ostream& operator<<(std::ostream& os, const Board& board) {
        const auto line_info = [&board](int line) -> std::string {
            if (line == 1)
                return "FEN: " + board.fen();
            if (line == 2)
                return "En passant: " + (board.ep_square.is_none() ? "-" : board.ep_square.str());
            return "";
        };

        os << "\u250c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n";

        for (int rank = 7; rank >= 0; rank--) {
            os << "\u2502 ";
            for (int file = 0; file < 8; file++) {
                const auto sq = static_cast<Square>(rank * 8 + file);
                const auto fgColor = board.pieces(WHITE).read_sq(sq) ? fmt::color::orange : fmt::color::dark_blue;

                os << fmt::format(fmt::fg(fgColor), "{}", board.read_sq_char(sq)) << " ";
            }
            os << "\u2502 " << rank + 1 << "   " << line_info(8 - rank) << "\n";
        }
        os << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n";
        os << "  a b c d e f g h\n";
        return os;
    }
}