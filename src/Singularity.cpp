#include <iostream>

#include "bitboard.h"
#include "square.h"

#ifdef _WIN32
    #include <windows.h>
#endif

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
    std::cout << "Hello World!" << std::endl;

    auto bb = chess::f3.as_bb() | chess::a8.as_bb();
    std::cout << bb.get_lsb() << std::endl;

    return 0;
}