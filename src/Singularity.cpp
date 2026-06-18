#include <iostream>
#include <string>

#include "board.h"

#ifdef _WIN32
    #include <windows.h>
#endif

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    std::string fen;
    while (true) {
        std::getline(std::cin, fen);

        if (fen == "quit")
            return 0;

        std::cout << chess::Board(fen) << std::endl;
    }

    return 0;
}