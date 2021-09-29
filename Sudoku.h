//
// Created by pc on 21-09-21.
//

#ifndef _SUDOKU_H
#define _SUDOKU_H

#include "NumberVector.h"
#include "RowArray.h"

#include <array>

class Sudoku {

  public:
    explicit Sudoku(const std::string& string);

    Sudoku(const Sudoku& other) = default;

    [[nodiscard]] static Sudoku PRESET(size_t index);
    [[nodiscard]] bool          solve();
    [[nodiscard]] bool          isConsistent() const;
    [[nodiscard]] std::string   toString() const;

  private:
    [[nodiscard]] bool         isSolved() const;
    [[nodiscard]] bool         fillSingles();
    [[nodiscard]] bool         countingCheck() const;
    [[nodiscard]] bool         rowCountingCheck(uint8_t row) const;
    [[nodiscard]] bool         columnCountingCheck(uint8_t column) const;
    [[nodiscard]] bool         blockCountingCheck(uint8_t block) const;
    [[nodiscard]] bool         set(uint8_t row, uint8_t column, uint8_t value);
    [[nodiscard]] bool         solveWithSubstitutions(uint8_t row, uint8_t column);
    [[nodiscard]] bool         isFree(uint8_t row, uint8_t column) const;
    [[nodiscard]] bool         fillAllHiddenSingles(bool& singleWasFilled);
    [[nodiscard]] bool         fillNakedSingles(bool& singleWasFilled);
    [[nodiscard]] bool         fillHiddenSinglesInRows(bool& singleWasFilled);
    [[nodiscard]] bool         fillHiddenSinglesInColumns(bool& singleWasFilled);
    [[nodiscard]] bool         fillHiddenSinglesInBlocks(bool& singleWasFilled);
    [[nodiscard]] bool         findPointingSets(bool& wasUpdated);
    [[nodiscard]] bool         potentiallyRemovePossibility(uint8_t row, uint8_t column, uint8_t value);
    [[nodiscard]] bool         removeIfPresent(uint8_t row, uint8_t column, uint8_t value, bool& wasRemoved);
    [[nodiscard]] float        getReductionScore(uint8_t row, uint8_t column) const;
    [[nodiscard]] uint8_t      getMinimumBranchingAfterSetting(uint8_t row, uint8_t column, uint8_t value) const;
    [[nodiscard]] uint8_t      numberOfNakedSingledCreated(uint8_t row, uint8_t column, uint8_t value) const;
    [[nodiscard]] uint8_t      numberOfNakedSingledCreated(uint8_t row, uint8_t column) const;
    [[nodiscard]] NumberVector possibleIfFree(uint8_t row, uint8_t column) const;
    [[nodiscard]] NumberVector findSinglesInRow(uint8_t row) const;
    [[nodiscard]] NumberVector findSinglesInColumn(uint8_t column) const;
    [[nodiscard]] NumberVector findSinglesInBlock(uint8_t firstRowOfBlock, uint8_t firstColumnOfBlock) const;

    std::array<RowArray, 9>                    m_rows{};
    std::array<std::array<NumberVector, 9>, 9> m_possibleAtPosition = {{{NumberVector{}.invert()}}};
};

#endif //_SUDOKU_H
