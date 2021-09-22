#include "Sudoku.h"

#include <cassert>
#include <chrono>
#include <iostream>

int main() {
    size_t N     = 450;
    size_t total = 0;
    for (size_t dummy = 0; dummy != N; ++dummy) {
        Sudoku sudoku = Sudoku::preset(0);
        auto   start  = std::chrono::high_resolution_clock::now();
        std::cout << (sudoku.solve()) << "\n";
        auto stop     = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total += duration.count();
    }
    std::cout << total / N << "\n";
}