#include <iostream>
#include <string>

#include "movegen.h"
#include "utils.h"

#ifdef _WIN32
    #include <windows.h>
#endif

using namespace chess;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::cout << "Building movegen databases\r" << std::flush;

    movegen::initialize_movegen_databases();

    std::cout << "Databases initialized.     " << std::endl;

    std::string command;
    Board board;

    while (true) {
        std::getline(std::cin, command);
        std::vector<std::string> tokens = split(command, ' ');

        if (command == "uci") {
            std::cout << "id name Singularity" << std::endl;
            std::cout << "id author Quinniboi10" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        if (command == "isready")
            std::cout << "readyok" << std::endl;

        if (command == "d")
            std::cout << board << std::endl;
        if (command == "dd") {
            std::cout << "Check mask\n" << board.check_mask.str() << "\n\n";
            std::cout << "Checkers\n" << board.checkers.str() << "\n\n";
            std::cout << "Pinned\n" << board.pinned.str() << "\n\n";
        }

        if (tokens[0] == "position") {
            if (tokens[1] == "fen")
                board = Board(command.substr(13));
            else if (tokens[1] == "startpos")
                board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            else if (tokens[1] == "kiwipete")
                board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

            if (tokens.size() > 9 && tokens[8] == "moves") {
                for (int i = 9; i < tokens.size(); i++)
                    board = board.move(Move(board, tokens[i]));
            }
        }

        if (tokens[0] == "perft")
            movegen::perft(board, std::stoi(tokens[1]), false);
        if (tokens[0] == "bulk")
            movegen::perft(board, std::stoi(tokens[1]), true);

        if (command == "quit" || command == "exit")
            return 0;
    }

    return 0;
}