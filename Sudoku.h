//
// Created by pc on 21-09-21.
//

#ifndef _SUDOKU_H
#define _SUDOKU_H

#include "NumberVector.h"
#include "Row.h"
#include "RowArray.h"

#include <array>

class Sudoku {

  public:
    explicit Sudoku(const std::string& string);

    Sudoku(const Sudoku& other) = default;

    static Sudoku preset(size_t index);

    bool solve();

    std::string toString() const;

  private:
    bool set(uint8_t row, uint8_t column, uint8_t value);

    bool solveWithSubstitutions(uint8_t row, uint8_t column);

    bool isFree(uint8_t row, uint8_t column) const;

    bool fillAllHiddenSingles(bool& singleWasFilled);

    bool fillNakedSingles(bool& singleWasFilled);

    bool fillSingles();

    bool fillHiddenSinglesInRows(bool& singleWasFilled);

    bool fillHiddenSinglesInColumns(bool& singleWasFilled);

    bool fillHiddenSinglesInBlocks(bool& singleWasFilled);

    bool checkRestricted(bool& possibleWasRemoved);

    bool potentiallyRemove(uint8_t row, uint8_t column, uint8_t value);

    bool removeIfPresent(uint8_t row, uint8_t column, uint8_t value, bool& wasRemoved);

    uint8_t numberOfNakedSingledCreated(uint8_t row, uint8_t column, uint8_t value) const;

    uint8_t numberOfNakedSingledCreated(uint8_t row, uint8_t column) const;

    bool countingCheck() const;

    NumberVector possibleAtPositionIfFree(uint8_t row, uint8_t column) const;

    NumberVector findSinglesInRow(uint8_t row) const;

    NumberVector findSinglesInColumn(uint8_t column) const;

    NumberVector findSinglesInBlock(uint8_t firstRowOfBlock, uint8_t firstColumnOfBlock) const;

    std::array<RowArray, 9> m_rows{};

    std::array<std::array<NumberVector, 9>, 9> m_possibleAtPosition = {{{NumberVector{}.invert()}}};
};

#endif //_SUDOKU_H
