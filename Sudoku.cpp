//
// Created by pc on 21-09-21.
//

#include "Sudoku.h"

#include "SudokuStaticArrays.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>

Sudoku::Sudoku(const std::string& string) {
    assert(string.length() == 81);
    for (uint8_t row = 0; row != 9; ++row) {
        m_rows[row] = RowArray(string.substr(9 * row, 9));
    }
    std::array<NumberVector, 9> numbersInRow;
    std::array<NumberVector, 9> numbersInColumn;
    std::array<NumberVector, 9> numbersInBlock;
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            numbersInColumn[column].add(m_rows[row].numberAt(column));
            numbersInBlock[INDICES_TO_BLOCK[row][column]].add(m_rows[row].numberAt(column));
            numbersInRow[row].add(m_rows[row].numberAt(column));
        }
    }
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                m_possibleAtPosition[row][column] = (numbersInColumn[column] | numbersInRow[row] | numbersInBlock[INDICES_TO_BLOCK[row][column]]).invert();
            }
        }
    }
}

std::string Sudoku::toString() const {
    std::stringstream ss;
    for (const auto& row : m_rows) {
        for (uint8_t j = 0; j != 9; ++j) {
            uint8_t value = row.numberAt(j);
            if (value == 0) {
                ss << ".";
            } else {
                ss << static_cast<unsigned short>(value);
            }
        }
        ss << "\n";
    }
    return ss.str();
}

bool Sudoku::potentiallyRemovePossibility(uint8_t row, uint8_t column, uint8_t value) {
    if (isFree(row, column)) {
        m_possibleAtPosition[row][column].remove(value);
        if (m_possibleAtPosition[row][column].count() == 0) {
            return false;
        }
        //        else if (m_possibleAtPosition[row][column].count() == 1) {
        //            const auto forcedValue = m_possibleAtPosition[row][column].smallestNumber();
        //            if (not set(row, column, forcedValue)) {
        //                return false;
        //            }
        //        }
    }
    return true;
}

bool Sudoku::set(uint8_t row, uint8_t column, uint8_t value) {
    assert(row < 9);
    m_rows[row].set(column, value);
    for (const auto& [r, c] : LINKED[row][column]) {
        m_possibleAtPosition[r][c].remove(value);
        if (isFree(r, c) && m_possibleAtPosition[r][c].count() == 0) {
            return false;
        }
    }
    for (const auto& [r, c] : LINKED[row][column]) {
        if (isFree(r, c) && m_possibleAtPosition[r][c].count() == 1 && not set(r, c, m_possibleAtPosition[r][c].smallestNumber())) {
            return false;
        }
    }
    return true;
}

bool Sudoku::isFree(uint8_t row, uint8_t column) const {
    assert(row < 9);
    assert(column < 9);
    return m_rows[row].positionIsFree(column);
}

bool Sudoku::fillNakedSingles(bool& singleWasFilled) {
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                const auto&   current = m_possibleAtPosition[row][column];
                const uint8_t count   = current.count();
                if (count == 1) {
                    if (not set(row, column, current.smallestNumber())) {
                        return false;
                    }
                    singleWasFilled = true;
                }
            }
        }
    }
    return true;
}

bool Sudoku::fillSingles() {
    bool wasUpdated = true;
    while (wasUpdated) {
        wasUpdated = false;
        if ((not fillNakedSingles(wasUpdated)) || (not fillAllHiddenSingles(wasUpdated))) {
            return false;
        }
    }
    return true;
}

bool Sudoku::solve() {
    if (not fillSingles()) {
        return false;
    }
    if (not countingCheck()) {
        return false;
    }
    bool wasUpdated = false;
    if (not findPointingSets(wasUpdated)) {
        return false;
    }
    uint8_t minimumPossible   = std::numeric_limits<uint8_t>::max();
    uint8_t minRow            = std::numeric_limits<uint8_t>::max();
    uint8_t minColumn         = std::numeric_limits<uint8_t>::max();
    float   minReductionScore = std::numeric_limits<float>::max();
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                const uint8_t count = m_possibleAtPosition[row][column].count();
                if (count <= minimumPossible) {
                    const float reductionScore = getReductionScore(row, column);
                    if (count < minimumPossible || reductionScore < minReductionScore) {
                        minimumPossible   = count;
                        minRow            = row;
                        minColumn         = column;
                        minReductionScore = reductionScore;
                    }
                }
            }
        }
    }
    if (minimumPossible == std::numeric_limits<uint8_t>::max()) {
        assert(isSolved());
        return true;
    }
    return solveWithSubstitutions(minRow, minColumn);
}

bool Sudoku::solveWithSubstitutions(uint8_t row, uint8_t column) {
    assert(isFree(row, column));
    const auto& allPossibilities = m_possibleAtPosition[row][column].allEntries();
    for (const auto number : allPossibilities) {
        Sudoku copy = *this;
        if (copy.set(row, column, number) && copy.solve()) {
            return true;
        }
    }
    return false;
}

Sudoku Sudoku::PRESET(size_t index) {
    switch (index) {
        case 0:
            return Sudoku{"123456789"
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."};
        case 1:
            return Sudoku{"53..7...."
                          "6..195..."
                          ".98....6."
                          "8...6...3"
                          "4..8.3..1"
                          "7...2...6"
                          ".6....28."
                          "...419..5"
                          "....8..79"};
        case 2:
            return Sudoku{"...4.8..."
                          ".6..7..1."
                          "7.2.9.5.4"
                          ".9.7.4.3."
                          "..7.5.8.."
                          ".8.9.6.5."
                          "9.4.1.7.8"
                          ".7..6..4."
                          "...2.7..."};
        case 3:
            return Sudoku{"53..7...."
                          "6........"
                          ".98....6."
                          "8.......3"
                          "4.......1"
                          "7.......6"
                          ".6.....8."
                          "........5"
                          "....8...."};
        case 4:
            return Sudoku{".....8..."
                          ".6..7..1."
                          "7.2.9.5.4"
                          ".9.7.4.3."
                          "..7.5...."
                          "...9.6.5."
                          "..4.1...8"
                          ".7..6...."
                          "...2.7..."};
        default:
            exit(1);
    }
}

static NumberVector findSingles(const std::array<NumberVector, 9>& numbers) {
    NumberVector tooMany;
    NumberVector result;
    for (const auto& number : numbers) {
        tooMany |= (number & result);
        result |= number;
    }
    return result & (tooMany.invert());
}

NumberVector Sudoku::findSinglesInRow(uint8_t row) const {
    NumberVector tooMany;
    NumberVector result;
    const auto&  possibleInRow = m_possibleAtPosition[row];
    for (uint8_t column = 0; column != 9; ++column) {
        if (isFree(row, column)) {
            tooMany |= (possibleInRow[column] & result);
            result |= possibleInRow[column];
        }
    }
    return result & (tooMany.invert());
}

bool Sudoku::fillAllHiddenSingles(bool& singleWasFilled) {
    return fillHiddenSinglesInRows(singleWasFilled) && fillHiddenSinglesInColumns(singleWasFilled) && fillHiddenSinglesInBlocks(singleWasFilled);
}

bool Sudoku::fillHiddenSinglesInRows(bool& singleWasFilled) {
    for (uint8_t row = 0; row != 9; ++row) {
        const NumberVector singles = findSinglesInRow(row);
        for (uint8_t column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                const auto singlesInPosition = singles & m_possibleAtPosition[row][column];
                if (singlesInPosition.count() == 1) {
                    if (not set(row, column, singlesInPosition.smallestNumber())) {
                        return false;
                    } else {
                        singleWasFilled = true;
                    }
                }
            }
        }
    }
    return true;
}

bool Sudoku::fillHiddenSinglesInColumns(bool& singleWasFilled) {
    for (uint8_t column = 0; column != 9; ++column) {
        const NumberVector singles = findSinglesInColumn(column);
        for (uint8_t row = 0; row != 9; ++row) {
            if (isFree(row, column)) {
                const auto singlesInPosition = singles & m_possibleAtPosition[row][column];
                if (singlesInPosition.count() == 1) {
                    if (not set(row, column, singlesInPosition.smallestNumber())) {
                        return false;
                    } else {
                        singleWasFilled = true;
                    }
                }
            }
        }
    }
    return true;
}

NumberVector Sudoku::findSinglesInColumn(uint8_t column) const {
    return findSingles({possibleIfFree(0, column),
                        possibleIfFree(1, column),
                        possibleIfFree(2, column),
                        possibleIfFree(3, column),
                        possibleIfFree(4, column),
                        possibleIfFree(5, column),
                        possibleIfFree(6, column),
                        possibleIfFree(7, column),
                        possibleIfFree(8, column)});
}

NumberVector Sudoku::findSinglesInBlock(uint8_t firstRowOfBlock, uint8_t firstColumnOfBlock) const {
    return findSingles({possibleIfFree(firstRowOfBlock, firstColumnOfBlock),
                        possibleIfFree(firstRowOfBlock + 1, firstColumnOfBlock),
                        possibleIfFree(firstRowOfBlock + 2, firstColumnOfBlock),
                        possibleIfFree(firstRowOfBlock, firstColumnOfBlock + 1),
                        possibleIfFree(firstRowOfBlock + 1, firstColumnOfBlock + 1),
                        possibleIfFree(firstRowOfBlock + 2, firstColumnOfBlock + 1),
                        possibleIfFree(firstRowOfBlock, firstColumnOfBlock + 2),
                        possibleIfFree(firstRowOfBlock + 1, firstColumnOfBlock + 2),
                        possibleIfFree(firstRowOfBlock + 2, firstColumnOfBlock + 2)});
}

bool Sudoku::fillHiddenSinglesInBlocks(bool& singleWasFilled) {
    for (uint8_t block = 0; block != 9; ++block) {
        const NumberVector singles = findSinglesInBlock(DIV_THREE_TIMES_THREE[block], TIMES_THREE_MOD_NINE[block]);
        for (const auto& [column, row] : BLOCK_POSITIONS[block]) {
            if (isFree(row, column)) {
                const auto singlesInPosition = singles & m_possibleAtPosition[row][column];
                if (singlesInPosition.count() == 1) {
                    if (not set(row, column, singlesInPosition.smallestNumber())) {
                        return false;
                    } else {
                        singleWasFilled = true;
                    }
                }
            }
        }
    }
    return true;
}

uint8_t Sudoku::numberOfNakedSingledCreated(uint8_t row, uint8_t column, uint8_t value) const {
    uint8_t count = 0;
    for (uint8_t index = 0; index != 9; ++index) {
        count += (isFree(row, index) && m_possibleAtPosition[row][index].count() == 2 && m_possibleAtPosition[row][index].contains(value));
        count += (isFree(index, column) && m_possibleAtPosition[index][column].count() == 2 && m_possibleAtPosition[index][column].contains(value));
    }
    for (const auto& [c, r] : BLOCK_POSITIONS[INDICES_TO_BLOCK[row][column]]) {
        count += (isFree(r, c) && m_possibleAtPosition[r][c].count() == 2 && m_possibleAtPosition[r][c].contains(value));
    }
    return count - 3 * (m_possibleAtPosition[row][column].count() == 2);
}

uint8_t Sudoku::numberOfNakedSingledCreated(uint8_t row, uint8_t column) const {
    assert(isFree(row, column));
    uint8_t result = 0;
    for (const auto number : m_possibleAtPosition[row][column].allEntries()) {
        result += numberOfNakedSingledCreated(row, column, number);
    }
    return result;
}

bool Sudoku::rowCountingCheck(uint8_t row) const {
    NumberVector allPossibleInRow;
    uint8_t      freeInRow = 0;
    for (uint8_t column = 0; column != 9; ++column) {
        if (isFree(row, column)) {
            allPossibleInRow |= m_possibleAtPosition[row][column];
            ++freeInRow;
            if (allPossibleInRow.count() < freeInRow) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::columnCountingCheck(uint8_t column) const {
    NumberVector allPossibleInColumn;
    uint8_t      freeInColumn = 0;
    for (uint8_t row = 0; row != 9; ++row) {
        if (isFree(row, column)) {
            allPossibleInColumn |= m_possibleAtPosition[row][column];
            ++freeInColumn;
            if (allPossibleInColumn.count() < freeInColumn) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::blockCountingCheck(uint8_t block) const {
    NumberVector allPossibleInBlock;
    uint8_t      freeInBlock = 0;
    for (const auto& [row, column] : BLOCK_POSITIONS[block]) {
        if (isFree(row, column)) {
            allPossibleInBlock |= m_possibleAtPosition[row][column];
            ++freeInBlock;
            if (allPossibleInBlock.count() < freeInBlock) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::countingCheck() const {
    for (uint8_t index = 0; index != 9; ++index) {
        if ((not rowCountingCheck(index)) || (not columnCountingCheck(index)) || (not blockCountingCheck(index))) {
            return false;
        }
    }
    return true;
}

NumberVector Sudoku::possibleIfFree(uint8_t row, uint8_t column) const {
    if (isFree(row, column)) {
        return m_possibleAtPosition[row][column];
    }
    return NumberVector{};
}

bool Sudoku::findPointingSets(bool& wasUpdated) {
    for (uint8_t row = 0; row < 9; row += 3) {
        for (uint8_t column = 0; column < 9; column += 3) {
            const NumberVector possibleInFirstRow  = possibleIfFree(row, column) | possibleIfFree(row, column + 1) | possibleIfFree(row, column + 2);
            const NumberVector possibleInSecondRow = possibleIfFree(row + 1, column) | possibleIfFree(row + 1, column + 1) | possibleIfFree(row + 1, column + 2);
            const NumberVector possibleInThirdRow  = possibleIfFree(row + 2, column) | possibleIfFree(row + 2, column + 1) | possibleIfFree(row + 2, column + 2);

            const NumberVector uniqueInRow[3]{possibleInFirstRow & (possibleInSecondRow.invert()) & (possibleInThirdRow.invert()),
                                              possibleInSecondRow & (possibleInFirstRow.invert()) & (possibleInThirdRow.invert()),
                                              possibleInThirdRow & (possibleInSecondRow.invert()) & (possibleInFirstRow.invert())};
            for (uint8_t rowOffset = 0; rowOffset != 3; ++rowOffset) {
                for (const auto value : uniqueInRow[rowOffset].allEntries()) {
                    for (uint8_t columnToRemove = 0; columnToRemove != 9; ++columnToRemove) {
                        if (columnToRemove < column || columnToRemove > column + 2) {
                            if (not removeIfPresent(row + rowOffset, columnToRemove, value, wasUpdated)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    for (uint8_t row = 0; row != 9; ++row) {
        const NumberVector possibleInFirstBlock  = possibleIfFree(row, 0) | possibleIfFree(row, 1) | possibleIfFree(row, 2);
        const NumberVector possibleInSecondBlock = possibleIfFree(row, 3) | possibleIfFree(row, 4) | possibleIfFree(row, 5);
        const NumberVector possibleInThirdBlock  = possibleIfFree(row, 6) | possibleIfFree(row, 7) | possibleIfFree(row, 8);
        const NumberVector uniquePerBlock[3]     = {possibleInFirstBlock & (possibleInSecondBlock.invert()) & (possibleInThirdBlock.invert()),
                                                possibleInSecondBlock & (possibleInFirstBlock.invert()) & (possibleInThirdBlock.invert()),
                                                possibleInThirdBlock & (possibleInSecondBlock.invert()) & (possibleInFirstBlock.invert())};
        for (uint8_t columnBlock = 0; columnBlock != 3; ++columnBlock) {
            for (const auto value : uniquePerBlock[columnBlock].allEntries()) {
                for (uint8_t rowToRemove = DIV_THREE_TIMES_THREE[row]; rowToRemove != DIV_THREE_TIMES_THREE[row] + 3; ++rowToRemove) {
                    if (rowToRemove != row) {
                        for (uint8_t column = 3 * columnBlock; column != 3 * columnBlock + 3; ++column) {
                            if (not removeIfPresent(rowToRemove, column, value, wasUpdated)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    for (uint8_t column = 0; column != 9; column += 3) {
        for (uint8_t row = 0; row != 9; row += 3) {
            const NumberVector possibleInFirstColumn  = possibleIfFree(row, column) | possibleIfFree(row + 1, column) | possibleIfFree(row + 2, column);
            const NumberVector possibleInSecondColumn = possibleIfFree(row, column + 1) | possibleIfFree(row + 1, column + 1) | possibleIfFree(row + 2, column + 1);
            const NumberVector possibleInThirdColumn  = possibleIfFree(row, column + 2) | possibleIfFree(row + 1, column + 2) | possibleIfFree(row + 2, column + 2);
            const NumberVector onlyInFirstColumn[3]   = {possibleInFirstColumn & (possibleInSecondColumn.invert()) & (possibleInThirdColumn.invert()),
                                                       possibleInSecondColumn & (possibleInFirstColumn.invert()) & (possibleInThirdColumn.invert()),
                                                       possibleInThirdColumn & (possibleInSecondColumn.invert()) & (possibleInFirstColumn.invert())};
            for (uint8_t k = 0; k != 3; ++k) {
                for (uint8_t rowToRemove = 0; rowToRemove != 9; ++rowToRemove) {
                    if (rowToRemove < row || rowToRemove > row + 2) {
                        for (const auto inFirst : onlyInFirstColumn[k].allEntries()) {
                            if (not removeIfPresent(rowToRemove, column + k, inFirst, wasUpdated)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool Sudoku::removeIfPresent(uint8_t row, uint8_t column, uint8_t value, bool& wasRemoved) {
    if (isFree(row, column)) {
        if (m_possibleAtPosition[row][column].contains(value)) {
            wasRemoved = true;
        }
        m_possibleAtPosition[row][column].remove(value);
        if (m_possibleAtPosition[row][column].count() == 0) {
            return false;
        } else if (m_possibleAtPosition[row][column].count() == 1) {
            const auto forcedValue = m_possibleAtPosition[row][column].smallestNumber();
            if (not set(row, column, forcedValue)) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::isSolved() const {
    for (uint8_t row = 0; row != 9; ++row) {
        std::set<uint8_t> allInRow;
        for (uint8_t column = 0; column != 9; ++column) {
            allInRow.insert(m_rows[row].numberAt(column));
        }
        if (allInRow.size() != 9 || *std::max_element(allInRow.begin(), allInRow.end()) > 9 || *std::min_element(allInRow.begin(), allInRow.end()) == 0) {
            std::cout << "Row error in row " << static_cast<int>(row) << "\n";
            return false;
        }
    }
    for (uint8_t column = 0; column != 9; ++column) {
        std::set<uint8_t> allInColumn;
        for (uint8_t row = 0; row != 9; ++row) {
            allInColumn.insert(m_rows[row].numberAt(column));
        }
        if (allInColumn.size() != 9 || *std::max_element(allInColumn.begin(), allInColumn.end()) > 9 || *std::min_element(allInColumn.begin(), allInColumn.end()) == 0) {
            std::cout << "Column error in column " << static_cast<int>(column) << "\n";
            return false;
        }
    }
    for (uint8_t block = 0; block != 9; ++block) {
        std::set<uint8_t> allInColumn;
        for (const auto [row, column] : BLOCK_POSITIONS[block]) {
            allInColumn.insert(m_rows[row].numberAt(column));
        }
        if (allInColumn.size() != 9 || *std::max_element(allInColumn.begin(), allInColumn.end()) > 9 || *std::min_element(allInColumn.begin(), allInColumn.end()) == 0) {
            std::cout << "Block error in block " << static_cast<int>(block) << "\n";
            return false;
        }
    }
    return true;
}

bool Sudoku::isConsistent() const {
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                for (const auto& [r, c] : LINKED[row][column]) {
                    if (not isFree(r, c) && m_possibleAtPosition[row][column].contains(m_rows[r].numberAt(c))) {
                        return false;
                    }
                }
            } else {
                for (const auto& [r, c] : LINKED[row][column]) {
                    if (not isFree(r, c) && m_rows[r].numberAt(c) == m_rows[row].numberAt(column)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

static size_t min(size_t a, size_t b) {
    return a < b ? a : b;
}

static size_t square(size_t a) {
    return a * a * a;
}

float Sudoku::getReductionScore(uint8_t row, uint8_t column) const {
    assert(isFree(row, column));
    size_t result = 0;
    for (const auto value : m_possibleAtPosition[row][column].allEntries()) {
        result += square(getMinimumBranchingAfterSetting(row, column, value));
    }
    return static_cast<float>(result) / static_cast<float>(m_possibleAtPosition[row][column].count());
}

uint8_t Sudoku::getMinimumBranchingAfterSetting(uint8_t row, uint8_t column, uint8_t value) const {
    size_t minimumBranch = std::numeric_limits<size_t>::max();
    for (const auto& [r, c] : LINKED[row][column]) {
        if (isFree(r, c)) {
            minimumBranch = min(minimumBranch, m_possibleAtPosition[r][c].count() - m_possibleAtPosition[r][c].contains(value));
        }
    }
    return minimumBranch;
}
