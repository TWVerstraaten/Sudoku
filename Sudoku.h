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

    bool solve();

    std::string toString() const;

  private:
    bool set(unsigned short row, unsigned short column, unsigned short value);

    bool solveWithSubstitutions(unsigned short row, unsigned short column);

    bool isFree(unsigned short row, unsigned short column) const;

    bool fillAllHiddenSingles(bool& singleWasFilled);

    bool fillNakedSingles(bool& singleWasFilled);

    bool fillSingles();

    bool fillHiddenSinglesInRows(bool& singleWasFilled);

    bool fillHiddenSinglesInColumns(bool& singleWasFilled);

    bool fillHiddenSinglesInBlocks(bool& singleWasFilled);

    bool potentiallyRemove(unsigned short row, unsigned short column, unsigned short value);

    size_t numberOfNakedSingledCreated(unsigned short row, unsigned short column, unsigned short value);

    size_t numberOfNakedSingledCreated(unsigned short row, unsigned short column);

    NumberVector findSinglesInRow(unsigned short row) const;

    NumberVector findSinglesInColumn(unsigned short column) const;

    NumberVector findSinglesInBlock(unsigned short firstRowOfBlock, unsigned short firstColumnOfBlock) const;

    unsigned short getReductionCount(unsigned short row, unsigned short column) const;

    unsigned short getReductionCount(unsigned short row, unsigned short column, unsigned short value) const;

    std::array<Row, 9> m_rows{};

    std::array<std::array<NumberVector, 9>, 9> m_possibleAtPosition = {{{NumberVector{}.invert()}}};
};

#endif //_SUDOKU_H
