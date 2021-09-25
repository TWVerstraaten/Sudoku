#include "Sudoku.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>

int main() {
    NumberVector::INIT_ARRAYS();

    std::ifstream file("/home/pc/Desktop/clion/sudoku/tests");
    assert(file.is_open());
    size_t c;
    file >> c;
    std::cout << c << "\n";
    size_t      i = 0;
    std::string line;
    std::getline(file, line);
    auto start = std::chrono::high_resolution_clock::now();
    while (std::getline(file, line)) {
        Sudoku s{line.substr(0, 81)};
        if (not s.solve()) {
            std::cout << "Error\n";
        }
        ++i;
        if (i % 128 == 0) {
            const float frac = static_cast<float>(i) / c;
            std::cout << "\n"
                      << 100.0f * frac << "%\tEstimated total = "
                      << (1.0f / 60000000.f) * std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() / frac
                      << " minutes\n";
        }
        if (i >= c) {
            break;
        }
    }
    auto stop     = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "\nTotal duration = \t" << duration.count() / 1000000.0 << "\n";

    //    size_t N     = 500;
    //    size_t total = 0;
    //    for (size_t dummy = 0; dummy != N; ++dummy) {
    //        //        Sudoku sudoku = Sudoku::preset(2);
    //        Sudoku sudoku = Sudoku::preset(4);
    //        auto   start  = std::chrono::high_resolution_clock::now();
    //        std::cout << (sudoku.countSolutions()) << "\n";
    //        auto stop     = std::chrono::high_resolution_clock::now();
    //        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    //        total += duration.count();
    //    }
    //    std::cout << total / N << "\n";
}
