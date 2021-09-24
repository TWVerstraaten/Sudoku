//
// Created by pc on 21-09-21.
//

#ifndef _SUDOKU_H
#define _SUDOKU_H

#include "NumberVector.h"
#include "Row.h"

#include <array>

class Sudoku {

  public:
    explicit Sudoku(const std::string& string);

    Sudoku(const Sudoku& other) = default;

    static Sudoku preset(size_t index);

    size_t countSolutions();

    bool solve();

    std::string toString() const;

  private:
    static unsigned char indicesToBlock(unsigned char row, unsigned char column);

    void set(unsigned char row, unsigned char column, unsigned char value);

    size_t countWithSubstitutionsUsingCopy(unsigned char row, unsigned char column);

    bool solveWithSingleSubstitution(unsigned char row, unsigned char column, unsigned char onlyPossible);

    bool solveWithSubstitutionsUsingCopy(unsigned char row, unsigned char column, const NumberVector& numberVector);

    bool isFree(unsigned char row, unsigned char column) const;

    void findNextBestMove(unsigned char& minRow, unsigned char& minColumn, NumberVector& numberVector, unsigned char& possibleCount);

    void fillHiddenSingles();

    NumberVector findSinglesInRow(unsigned char row);

    std::array<Row, 9> m_rows{};

    std::array<std::array<NumberVector, 9>, 9> m_possibleAtPosition = {{{NumberVector{0}.invert()}}};
};

#endif //_SUDOKU_H
