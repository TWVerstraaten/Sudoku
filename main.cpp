#include "Sudoku.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono;

int main() {

    std::cout << std::thread::hardware_concurrency() << "\n";

    size_t N     = 150;
    size_t total = 0;
    for (size_t dummy = 0; dummy != N; ++dummy) {
        Sudoku sudoku = Sudoku::preset(0);
        auto   start  = high_resolution_clock::now();
        assert(sudoku.solve());
        auto stop     = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        total += duration.count();
    }
    std::cout << total / N << "\n";
}