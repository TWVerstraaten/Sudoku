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

    bool canBePlaced(unsigned short row, unsigned short column, unsigned short value);

    size_t countWithSubstitutionsUsingCopy(unsigned short row, unsigned short column);

    bool solveWithSingleSubstitution(unsigned short row, unsigned short column, unsigned short onlyPossible);

    bool solveWithSubstitutionsUsingCopy(unsigned short row, unsigned short column, const NumberVector& numberVector);

    bool isFree(unsigned short row, unsigned short column) const;

    void findNextBestMove(unsigned short& minRow, unsigned short& minColumn, NumberVector& numberVector, unsigned short& possibleCount);

    void fillHiddenSingles();

    NumberVector findSinglesInRow(unsigned short row);

    NumberVector numbersInColumn(unsigned short column) const;

    NumberVector numbersInRow(unsigned short row) const;

    NumberVector numbersInBlock(unsigned short block) const;

    NumberVector ruledOutAt(unsigned short row, unsigned short column) const;

    std::array<Row, 9>                                     m_rows{};
    std::array<NumberVector, 9>                            m_numbersInColumns;
    std::array<NumberVector, 9>                            m_numbersInBlocks;
    std::array<NumberVector, 9>                            m_numbersInRows;
    std::array<std::array<NumberVector, 9>, 9>             m_possibleAtPosition;
    std::array<std::vector<std::pair<size_t, size_t>>, 10> m_sortedPossible;
};

#endif //_SUDOKU_H
