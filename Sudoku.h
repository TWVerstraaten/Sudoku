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

    bool operator==(const Sudoku& other);

  private:
    static unsigned short indicesToBlock(unsigned short row, unsigned short column);

    void set(unsigned short row, unsigned short column, unsigned short value);

    size_t countWithSubstitutions(const NumberVector& numberVector, unsigned short row, unsigned short column);

    size_t countWithSingleSubstitution(unsigned short onlyPossible, unsigned short row, unsigned short column);

    size_t countWithSubstitutionsUsingCopy(const NumberVector& numberVector, unsigned short row, unsigned short column);

    bool solveWithSubstitutions(const NumberVector& numberVector, unsigned short row, unsigned short column);

    bool solveWithSingleSubstitution(unsigned short onlyPossible, unsigned short row, unsigned short column);

    bool solveWithSubstitutionsUsingCopy(const NumberVector& numberVector, unsigned short row, unsigned short column);

    bool isFree(unsigned short row, unsigned short column) const;

    void findNextBestMove(unsigned short& minRow, unsigned short& minColumn, NumberVector& numberVector, unsigned short& missing);

    void fillHiddenSingles();

    NumberVector numbersInColumn(unsigned short column) const;

    NumberVector numbersInRow(unsigned short row) const;

    NumberVector numbersInBlock(unsigned short block) const;

    NumberVector ruledOutAt(unsigned short row, unsigned short column) const;

    std::array<Row, 9>          m_rows{};
    std::array<NumberVector, 9> m_numbersInColumns;
    std::array<NumberVector, 9> m_numbersInBlocks;
    std::array<NumberVector, 9> m_numbersInRows;
};

#endif //_SUDOKU_H
