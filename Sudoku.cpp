//
// Created by pc on 21-09-21.
//

#include "Sudoku.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <sstream>

static const std::pair<uint8_t, uint8_t> BLOCK_POSITIONS[9][9] = {{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}},
                                                                  {{3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 1}, {4, 2}, {5, 0}, {5, 1}, {5, 2}},
                                                                  {{6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 1}, {7, 2}, {8, 0}, {8, 1}, {8, 2}},
                                                                  {{0, 3}, {0, 4}, {0, 5}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}},
                                                                  {{3, 3}, {3, 4}, {3, 5}, {4, 3}, {4, 4}, {4, 5}, {5, 3}, {5, 4}, {5, 5}},
                                                                  {{6, 3}, {6, 4}, {6, 5}, {7, 3}, {7, 4}, {7, 5}, {8, 3}, {8, 4}, {8, 5}},
                                                                  {{0, 6}, {0, 7}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 6}, {2, 7}, {2, 8}},
                                                                  {{3, 6}, {3, 7}, {3, 8}, {4, 6}, {4, 7}, {4, 8}, {5, 6}, {5, 7}, {5, 8}},
                                                                  {{6, 6}, {6, 7}, {6, 8}, {7, 6}, {7, 7}, {7, 8}, {8, 6}, {8, 7}, {8, 8}}};

static const uint8_t TIMES_THREE_MOD_NINE[9]  = {0, 3, 6, 0, 3, 6, 0, 3, 6};
static const uint8_t DIV_THREE_TIMES_THREE[9] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
static const uint8_t INDICES_TO_BLOCK[9][9]   = {{0, 0, 0, 1, 1, 1, 2, 2, 2},
                                               {0, 0, 0, 1, 1, 1, 2, 2, 2},
                                               {0, 0, 0, 1, 1, 1, 2, 2, 2},
                                               {3, 3, 3, 4, 4, 4, 5, 5, 5},
                                               {3, 3, 3, 4, 4, 4, 5, 5, 5},
                                               {3, 3, 3, 4, 4, 4, 5, 5, 5},
                                               {6, 6, 6, 7, 7, 7, 8, 8, 8},
                                               {6, 6, 6, 7, 7, 7, 8, 8, 8},
                                               {6, 6, 6, 7, 7, 7, 8, 8, 8}};

Sudoku::Sudoku(const std::string& string) {
    assert(string.length() == 81);
    for (uint8_t row = 0; row != 9; ++row) {
        m_rows[row] = RowArray(string.substr(9 * row, 9));
    }
    std::array<NumberVector, 9> numbersInColumns;
    std::array<NumberVector, 9> numbersInBlocks;
    std::array<NumberVector, 9> numbersInRows;
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            numbersInColumns[column].add(m_rows[row].numberAt(column));
            numbersInBlocks[INDICES_TO_BLOCK[row][column]].add(m_rows[row].numberAt(column));
            numbersInRows[row].add(m_rows[row].numberAt(column));
        }
    }
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            m_possibleAtPosition[row][column] = (numbersInColumns[column] | numbersInRows[row] | numbersInBlocks[INDICES_TO_BLOCK[row][column]]).invert();
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

bool Sudoku::potentiallyRemove(uint8_t row, uint8_t column, uint8_t value) {
    if (isFree(row, column)) {
        m_possibleAtPosition[row][column].remove(value);
        if (m_possibleAtPosition[row][column].isEmpty()) {
            return false;
        }
    }
    return true;
}

bool Sudoku::set(uint8_t row, uint8_t column, uint8_t value) {
    assert(row < 9);
    m_rows[row].set(column, value);
    for (uint8_t index = 0; index != 9; ++index) {
        // Remove possibility from row and column
        if (not(potentiallyRemove(row, index, value) && potentiallyRemove(index, column, value))) {
            return false;
        }
    }
    // Remove possibility from block
    for (const auto& [r, c] : BLOCK_POSITIONS[INDICES_TO_BLOCK[column][row]]) {
        if (not potentiallyRemove(r, c, value)) {
            return false;
        }
    }
    for (uint8_t index = 0; index != 9; ++index) {
        // Set singles in row
        if (m_possibleAtPosition[row][index].count() == 1 && isFree(row, index) && not set(row, index, m_possibleAtPosition[row][index].smallestNumber())) {
            return false;
        }
        // Set singles in column
        if (m_possibleAtPosition[index][column].count() == 1 && isFree(index, column) && not set(index, column, m_possibleAtPosition[index][column].smallestNumber())) {
            return false;
        }
    }
    // Set singles in block
    for (const auto& [r, c] : BLOCK_POSITIONS[INDICES_TO_BLOCK[column][row]]) {
        if (m_possibleAtPosition[r][c].count() == 1 && isFree(r, c) && not set(r, c, m_possibleAtPosition[r][c].smallestNumber())) {
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
            if (not isFree(row, column)) {
                continue;
            }
            const auto&   current = m_possibleAtPosition[row][column];
            const uint8_t count   = current.count();
            if (count == 0) {
                return false;
            } else if (count == 1) {
                if (not set(row, column, current.smallestNumber())) {
                    return false;
                }
                singleWasFilled = true;
            }
        }
    }
    return true;
}

bool Sudoku::fillSingles() {
    bool wasUpdated = true;
    while (wasUpdated) {
        wasUpdated = false;
        if ((not fillAllHiddenSingles(wasUpdated)) || (not fillNakedSingles(wasUpdated)) || (not checkRestricted(wasUpdated))) {
            return false;
        }
    }
    return true;
}

bool Sudoku::solve() {
    if (not countingCheck() || not fillSingles()) {
        return false;
    }
    uint8_t minimumPossible            = std::numeric_limits<uint8_t>::max();
    uint8_t minRow                     = std::numeric_limits<uint8_t>::max();
    uint8_t minColumn                  = std::numeric_limits<uint8_t>::max();
    uint8_t maximumNakedSinglesCreated = 0;
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (not isFree(row, column)) {
                continue;
            }
            const auto&   current = m_possibleAtPosition[row][column];
            const uint8_t count   = current.count();
            if (count <= minimumPossible) {
                const uint8_t nakedSinglesCreated = numberOfNakedSingledCreated(row, column);
                if (count < minimumPossible || nakedSinglesCreated > maximumNakedSinglesCreated) {
                    minimumPossible            = count;
                    minRow                     = row;
                    minColumn                  = column;
                    maximumNakedSinglesCreated = nakedSinglesCreated;
                }
            }
        }
    }
    if (minimumPossible == std::numeric_limits<uint8_t>::max()) {
        //        std::cout << toString() << "\n";
        return true;
    }
    return solveWithSubstitutions(minRow, minColumn);
}

uint8_t reductionWeight(uint8_t numPossible) {
    return 20 - numPossible;
}

bool Sudoku::solveWithSubstitutions(uint8_t row, uint8_t column) {
    assert(isFree(row, column));
    auto allPossibilities = m_possibleAtPosition[row][column].allEntries();
    std::sort(allPossibilities.begin(), allPossibilities.end(), [&](uint8_t first, uint8_t second) {
        return numberOfNakedSingledCreated(row, column, first) < numberOfNakedSingledCreated(row, column, second);
    });
    for (const auto number : allPossibilities) {
        Sudoku copy = *this;
        if (copy.set(row, column, number) && copy.solve()) {
            return true;
        }
    }
    return false;
}

Sudoku Sudoku::preset(size_t index) {
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
    return findSingles({possibleAtPositionIfFree(0, column),
                        possibleAtPositionIfFree(1, column),
                        possibleAtPositionIfFree(2, column),
                        possibleAtPositionIfFree(3, column),
                        possibleAtPositionIfFree(4, column),
                        possibleAtPositionIfFree(5, column),
                        possibleAtPositionIfFree(6, column),
                        possibleAtPositionIfFree(7, column),
                        possibleAtPositionIfFree(8, column)});
}

NumberVector Sudoku::findSinglesInBlock(uint8_t firstRowOfBlock, uint8_t firstColumnOfBlock) const {
    return findSingles({possibleAtPositionIfFree(firstRowOfBlock, firstColumnOfBlock),
                        possibleAtPositionIfFree(firstRowOfBlock + 1, firstColumnOfBlock),
                        possibleAtPositionIfFree(firstRowOfBlock + 2, firstColumnOfBlock),
                        possibleAtPositionIfFree(firstRowOfBlock, firstColumnOfBlock + 1),
                        possibleAtPositionIfFree(firstRowOfBlock + 1, firstColumnOfBlock + 1),
                        possibleAtPositionIfFree(firstRowOfBlock + 2, firstColumnOfBlock + 1),
                        possibleAtPositionIfFree(firstRowOfBlock, firstColumnOfBlock + 2),
                        possibleAtPositionIfFree(firstRowOfBlock + 1, firstColumnOfBlock + 2),
                        possibleAtPositionIfFree(firstRowOfBlock + 2, firstColumnOfBlock + 2)});
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

bool Sudoku::countingCheck() const {
    for (uint8_t row = 0; row != 9; ++row) {
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
    }
    for (uint8_t column = 0; column != 9; ++column) {
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
    }
    for (const auto& block : BLOCK_POSITIONS) {
        NumberVector allPossibleInBlock;
        uint8_t      freeInBlock = 0;
        for (const auto& [row, column] : block) {
            if (isFree(row, column)) {
                allPossibleInBlock |= m_possibleAtPosition[row][column];
                ++freeInBlock;
                if (allPossibleInBlock.count() < freeInBlock) {
                    return false;
                }
            }
        }
    }
    return true;
}

NumberVector Sudoku::possibleAtPositionIfFree(uint8_t row, uint8_t column) const {
    if (isFree(row, column)) {
        return m_possibleAtPosition[row][column];
    }
    return NumberVector{};
}

bool Sudoku::checkRestricted(bool& possibleWasRemoved) {
    for (uint8_t r = 0; r < 9; r += 3) {
        for (uint8_t c = 0; c < 9; c += 3) {
            NumberVector possibleInFirstRow  = possibleAtPositionIfFree(r, c) | possibleAtPositionIfFree(r, c + 1) | possibleAtPositionIfFree(r, c + 2);
            NumberVector possibleInSecondRow = possibleAtPositionIfFree(r + 1, c) | possibleAtPositionIfFree(r + 1, c + 1) | possibleAtPositionIfFree(r + 1, c + 2);
            NumberVector possibleInThirdRow  = possibleAtPositionIfFree(r + 2, c) | possibleAtPositionIfFree(r + 2, c + 1) | possibleAtPositionIfFree(r + 2, c + 2);

            NumberVector onlyInFirstRow  = possibleInFirstRow & (possibleInSecondRow.invert()) & (possibleInThirdRow.invert());
            NumberVector onlyInSecondRow = possibleInSecondRow & (possibleInFirstRow.invert()) & (possibleInThirdRow.invert());
            NumberVector onlyInThirdRow  = possibleInThirdRow & (possibleInSecondRow.invert()) & (possibleInFirstRow.invert());
            for (uint8_t column = 0; column != 9; ++column) {
                if (column < c || column > c + 2) {
                    for (const auto& inFirst : onlyInFirstRow.allEntries()) {
                        if (not removeIfPresent(r, column, inFirst, possibleWasRemoved)) {
                            return false;
                        }
                    }
                    for (const auto& inSecond : onlyInSecondRow.allEntries()) {
                        if (not removeIfPresent(r + 1, column, inSecond, possibleWasRemoved)) {
                            return false;
                        }
                    }
                    for (const auto& inThird : onlyInThirdRow.allEntries()) {
                        if (not removeIfPresent(r + 2, column, inThird, possibleWasRemoved)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    for (uint8_t c = 0; c != 9; c += 3) {
        for (uint8_t r = 0; r != 9; r += 3) {
            NumberVector possibleInFirstColumn  = possibleAtPositionIfFree(r, c) | possibleAtPositionIfFree(r + 1, c) | possibleAtPositionIfFree(r + 2, c);
            NumberVector possibleInSecondColumn = possibleAtPositionIfFree(r, c + 1) | possibleAtPositionIfFree(r + 1, c + 1) | possibleAtPositionIfFree(r + 2, c + 1);
            NumberVector possibleInThirdColumn  = possibleAtPositionIfFree(r, c + 2) | possibleAtPositionIfFree(r + 1, c + 2) | possibleAtPositionIfFree(r + 2, c + 2);
            NumberVector onlyInFirstColumn      = possibleInFirstColumn & (possibleInSecondColumn.invert()) & (possibleInThirdColumn.invert());
            NumberVector onlyInSecondColumn     = possibleInSecondColumn & (possibleInFirstColumn.invert()) & (possibleInThirdColumn.invert());
            NumberVector onlyInThirdColumn      = possibleInThirdColumn & (possibleInSecondColumn.invert()) & (possibleInFirstColumn.invert());
            for (uint8_t row = 0; row != 9; ++row) {
                if (row < r || row > r + 2) {
                    for (const auto inFirst : onlyInFirstColumn.allEntries()) {
                        if (not removeIfPresent(row, c, inFirst, possibleWasRemoved)) {
                            return false;
                        }
                    }
                    for (const auto inFirst : onlyInSecondColumn.allEntries()) {
                        if (not removeIfPresent(row, c + 1, inFirst, possibleWasRemoved)) {
                            return false;
                        }
                    }
                    for (const auto inFirst : onlyInThirdColumn.allEntries()) {
                        if (not removeIfPresent(row, c + 2, inFirst, possibleWasRemoved)) {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool Sudoku::removeIfPresent(uint8_t row, uint8_t column, uint8_t value, bool& wasRemoved) {
    if (isFree(row, column) && m_possibleAtPosition[row][column].contains(value)) {
        wasRemoved = true;
        m_possibleAtPosition[row][column].remove(value);
        if (m_possibleAtPosition[row][column].count() == 1) {
            return set(row, column, m_possibleAtPosition[row][column].smallestNumber());
        }
    }
    return true;
}
