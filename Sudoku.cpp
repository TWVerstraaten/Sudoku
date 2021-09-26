//
// Created by pc on 21-09-21.
//

#include "Sudoku.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <memory>
#include <set>
#include <sstream>

static const unsigned short DIV_THREE_TIMES_THREE[9] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
static const unsigned short INDICES_TO_BLOCK[9][9]   = {{0, 0, 0, 1, 1, 1, 2, 2, 2},
                                                      {0, 0, 0, 1, 1, 1, 2, 2, 2},
                                                      {0, 0, 0, 1, 1, 1, 2, 2, 2},
                                                      {3, 3, 3, 4, 4, 4, 5, 5, 5},
                                                      {3, 3, 3, 4, 4, 4, 5, 5, 5},
                                                      {3, 3, 3, 4, 4, 4, 5, 5, 5},
                                                      {6, 6, 6, 7, 7, 7, 8, 8, 8},
                                                      {6, 6, 6, 7, 7, 7, 8, 8, 8},
                                                      {6, 6, 6, 7, 7, 7, 8, 8, 8}};

static unsigned short max(unsigned short a, unsigned short b) {
    return a > b ? a : b;
}

Sudoku::Sudoku(const std::string& string) {
    assert(string.length() == 81);
    for (unsigned short row = 0; row != 9; ++row) {
        m_rows[row] = Row(string.substr(9 * row, 9));
    }

    std::array<NumberVector, 9> numbersInColumns;
    std::array<NumberVector, 9> numbersInBlocks;
    std::array<NumberVector, 9> numbersInRows;
    for (size_t row = 0; row != 9; ++row) {
        for (size_t column = 0; column != 9; ++column) {
            numbersInColumns[column].add(m_rows[row].numberAt(column));
            numbersInBlocks[INDICES_TO_BLOCK[row][column]].add(m_rows[row].numberAt(column));
            numbersInRows[row].add(m_rows[row].numberAt(column));
        }
    }
    for (size_t row = 0; row != 9; ++row) {
        for (size_t column = 0; column != 9; ++column) {
            m_possibleAtPosition[row][column] = (numbersInColumns[column] | numbersInRows[row] | numbersInBlocks[INDICES_TO_BLOCK[row][column]]).invert();
        }
    }
}

std::string Sudoku::toString() const {
    std::stringstream ss;
    for (const auto& row : m_rows) {
        for (unsigned short j = 0; j != 9; ++j) {
            unsigned short value = row.numberAt(j);
            if (value == 0) {
                ss << ".";
            } else {
                ss << value;
            }
        }
        ss << "\n";
    }
    return ss.str();
}

bool Sudoku::potentiallyRemove(unsigned short row, unsigned short column, unsigned short value) {
    if (isFree(row, column)) {
        m_possibleAtPosition[row][column].remove(value);
        if (m_possibleAtPosition[row][column].isEmpty()) {
            return false;
        }
    }
    return true;
}

bool Sudoku::set(unsigned short row, unsigned short column, unsigned short value) {
    assert(row < 9);
    m_rows[row].set(column, value);
    const unsigned short firstRowOfBlock    = DIV_THREE_TIMES_THREE[row];
    const unsigned short firstColumnOfBlock = DIV_THREE_TIMES_THREE[column];
    for (unsigned short index = 0; index != 9; ++index) {
        // Remove possibility from row and column
        if (not(potentiallyRemove(row, index, value) && potentiallyRemove(index, column, value))) {
            return false;
        }
    }
    // Remove possibility from block
    for (unsigned short r = firstRowOfBlock; r != firstRowOfBlock + 3; ++r) {
        for (unsigned short c = firstColumnOfBlock; c != firstColumnOfBlock + 3; ++c) {
            if (not potentiallyRemove(r, c, value)) {
                return false;
            }
        }
    }
    for (size_t index = 0; index != 9; ++index) {
        // Set singles in row
        if (isFree(row, index) && m_possibleAtPosition[row][index].count() == 1 && not set(row, index, m_possibleAtPosition[row][index].smallestNumber())) {
            return false;
        }
        // Set singles in column
        if (isFree(index, column) && m_possibleAtPosition[index][column].count() == 1 && not set(index, column, m_possibleAtPosition[index][column].smallestNumber())) {
            return false;
        }
        // Set singles in block
        const unsigned short rowInBlock    = firstRowOfBlock + index / 3;
        const unsigned short columnInBlock = firstColumnOfBlock + index % 3;
        if (isFree(rowInBlock, columnInBlock) && m_possibleAtPosition[rowInBlock][columnInBlock].count() == 1 &&
            not set(rowInBlock, columnInBlock, m_possibleAtPosition[rowInBlock][columnInBlock].smallestNumber())) {
            return false;
        }
    }
    return true;
}

bool Sudoku::isFree(unsigned short row, unsigned short column) const {
    assert(row < 9);
    assert(column < 9);
    return m_rows.at(row).positionIsFree(column);
}

bool Sudoku::fillNakedSingles(bool& singleWasFilled) {
    for (size_t row = 0; row != 9; ++row) {
        for (unsigned int column = 0; column != 9; ++column) {
            if (not isFree(row, column)) {
                continue;
            }
            const auto&          current = m_possibleAtPosition[row][column];
            const unsigned short count   = current.count();
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
    bool nakedSingleWasFilled  = true;
    bool hiddenSingleWasFilled = true;
    while (nakedSingleWasFilled || hiddenSingleWasFilled) {
        nakedSingleWasFilled  = false;
        hiddenSingleWasFilled = false;
        if ((not fillAllHiddenSingles(hiddenSingleWasFilled)) || (not fillNakedSingles(nakedSingleWasFilled))) {
            return false;
        }
    }
    return true;
}

bool Sudoku::solve() {
    if (not fillSingles()) {
        return false;
    }
    unsigned short minimumPossible            = std::numeric_limits<unsigned short>::max();
    unsigned short minRow                     = std::numeric_limits<unsigned short>::max();
    unsigned short minColumn                  = std::numeric_limits<unsigned short>::max();
    size_t         maximumNakedSinglesCreated = 0;
    for (size_t row = 0; row != 9; ++row) {
        for (unsigned int column = 0; column != 9; ++column) {
            if (not isFree(row, column)) {
                continue;
            }
            const auto&          current = m_possibleAtPosition[row][column];
            const unsigned short count   = current.count();
            if (count <= minimumPossible) {
                const unsigned short nakedSinglesCreated = numberOfNakedSingledCreated(row, column);
                if (count < minimumPossible || nakedSinglesCreated > maximumNakedSinglesCreated) {
                    minimumPossible            = count;
                    minRow                     = row;
                    minColumn                  = column;
                    maximumNakedSinglesCreated = nakedSinglesCreated;
                }
            }
        }
    }
    if (minimumPossible == std::numeric_limits<unsigned short>::max()) {
        //            std::cout << toString() << "\n";
        return true;
    }
    return solveWithSubstitutions(minRow, minColumn);
}

unsigned short reductionWeight(unsigned short numPossible) {
    return 20 - numPossible;
}

unsigned short Sudoku::getReductionCount(unsigned short row, unsigned short column, unsigned short value) const {
    unsigned short reductionCount = 0;
    for (unsigned short index = 0; index != 9; ++index) {
        if (isFree(row, index) && m_possibleAtPosition[row][index].contains(value)) {
            //            reductionCount += reductionWeight(m_possibleAtPosition[row][index].count());
            reductionCount = max(reductionWeight(m_possibleAtPosition[row][index].count()), reductionCount);
        }
        if (isFree(index, column) && m_possibleAtPosition[index][column].contains(value)) {
            reductionCount = max(reductionWeight(m_possibleAtPosition[index][column].count()), reductionCount);
            //            reductionCount += reductionWeight(m_possibleAtPosition[index][column].count());
        }
    }
    const unsigned short firstRowOfBlock    = DIV_THREE_TIMES_THREE[row];
    const unsigned short firstColumnOfBlock = DIV_THREE_TIMES_THREE[column];
    for (unsigned short r = firstRowOfBlock; r != firstRowOfBlock + 3; ++r) {
        for (unsigned short c = firstColumnOfBlock; c != firstColumnOfBlock + 3; ++c) {
            if (isFree(r, c) && m_possibleAtPosition[r][c].contains(value)) {
                //                reductionCount += reductionWeight(m_possibleAtPosition[r][c].count());
                reductionCount = max(reductionWeight(m_possibleAtPosition[r][c].count()), reductionCount);
            }
        }
    }
    //    return reductionCount - 3 * reductionWeight(m_possibleAtPosition[row][column].count());
    return reductionCount;
}

unsigned short Sudoku::getReductionCount(unsigned short row, unsigned short column) const {
    unsigned short reductionCount = 0;
    const auto&    possible       = m_possibleAtPosition[row][column];
    for (const auto el : possible.allEntries()) {
        const auto current = getReductionCount(row, column, el);
        reductionCount     = max(current, reductionCount);
    }
    return reductionCount;
}

bool Sudoku::solveWithSubstitutions(unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    auto       allPossibilities = m_possibleAtPosition[row][column].allEntries();
    const auto comparator       = [&](unsigned short first, unsigned short second) {
        return numberOfNakedSingledCreated(row, column, first) < numberOfNakedSingledCreated(row, column, second);
    };
    if (allPossibilities.size() > 1) {
        std::sort(allPossibilities.begin(), allPossibilities.end(), comparator);
    }
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

NumberVector Sudoku::findSinglesInRow(unsigned short row) const {
    NumberVector tooMany;
    NumberVector result;
    const auto&  possibleInRow = m_possibleAtPosition[row];
    for (unsigned short column = 0; column != 9; ++column) {
        if (isFree(row, column)) {
            tooMany |= (possibleInRow[column] & result);
            result |= possibleInRow[column];
        }
    }
    return result & (tooMany.invert());
}

bool Sudoku::fillAllHiddenSingles(bool& singleWasFilled) {
    //        for (unsigned short row = 0; row != 9; ++row) {
    //            for (unsigned short first = 1; first != 9; ++first) {
    //                if (m_possibleAtPosition[row][first].count() < 2) {
    //                    continue;
    //                }
    //                for (unsigned short second = 0; second != first; ++second) {
    //                    auto intersect = m_possibleAtPosition[row][first] & m_possibleAtPosition[row][second];
    //                    if (m_possibleAtPosition[row][first].count() < 2 || intersect.count() < 2) {
    //                        continue;
    //                    }
    //                    bool wasSeen = true;
    //                    for (size_t check = 0; wasSeen && check != 9; ++check) {
    //                        if (intersect.count() < 2) {
    //                            wasSeen = false;
    //                            continue;
    //                        }
    //                        if (check != first && check != second) {
    //                            intersect &= m_possibleAtPosition[row][check].invert();
    //                        }
    //                    }
    //                    if (wasSeen && intersect.count() == 2) {
    //                        m_possibleAtPosition[row][first]  = intersect;
    //                        m_possibleAtPosition[row][second] = intersect;
    //                    }
    //                }
    //            }
    //        }
    return fillHiddenSinglesInRows(singleWasFilled) && fillHiddenSinglesInColumns(singleWasFilled) && fillHiddenSinglesInBlocks(singleWasFilled);
}

bool Sudoku::fillHiddenSinglesInRows(bool& singleWasFilled) {
    for (unsigned short row = 0; row != 9; ++row) {
        const NumberVector singles = findSinglesInRow(row);
        for (unsigned short column = 0; column != 9; ++column) {
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
    for (unsigned short column = 0; column != 9; ++column) {
        const NumberVector singles = findSinglesInColumn(column);
        for (unsigned short row = 0; row != 9; ++row) {
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

NumberVector Sudoku::findSinglesInColumn(unsigned short column) const {
    return findSingles({isFree(0, column) ? m_possibleAtPosition[0][column] : 0,
                        isFree(1, column) ? m_possibleAtPosition[1][column] : 0,
                        isFree(2, column) ? m_possibleAtPosition[2][column] : 0,
                        isFree(3, column) ? m_possibleAtPosition[3][column] : 0,
                        isFree(4, column) ? m_possibleAtPosition[4][column] : 0,
                        isFree(5, column) ? m_possibleAtPosition[5][column] : 0,
                        isFree(6, column) ? m_possibleAtPosition[6][column] : 0,
                        isFree(7, column) ? m_possibleAtPosition[7][column] : 0,
                        isFree(8, column) ? m_possibleAtPosition[8][column] : 0});
}

NumberVector Sudoku::findSinglesInBlock(unsigned short firstRowOfBlock, unsigned short firstColumnOfBlock) const {
    return findSingles({isFree(firstRowOfBlock, firstColumnOfBlock) ? m_possibleAtPosition[firstRowOfBlock][firstColumnOfBlock] : 0,
                        isFree(firstRowOfBlock + 1, firstColumnOfBlock) ? m_possibleAtPosition[firstRowOfBlock + 1][firstColumnOfBlock] : 0,
                        isFree(firstRowOfBlock + 2, firstColumnOfBlock) ? m_possibleAtPosition[firstRowOfBlock + 2][firstColumnOfBlock] : 0,
                        isFree(firstRowOfBlock, firstColumnOfBlock + 1) ? m_possibleAtPosition[firstRowOfBlock][firstColumnOfBlock + 1] : 0,
                        isFree(firstRowOfBlock + 1, firstColumnOfBlock + 1) ? m_possibleAtPosition[firstRowOfBlock + 1][firstColumnOfBlock + 1] : 0,
                        isFree(firstRowOfBlock + 2, firstColumnOfBlock + 1) ? m_possibleAtPosition[firstRowOfBlock + 2][firstColumnOfBlock + 1] : 0,
                        isFree(firstRowOfBlock, firstColumnOfBlock + 2) ? m_possibleAtPosition[firstRowOfBlock][firstColumnOfBlock + 2] : 0,
                        isFree(firstRowOfBlock + 1, firstColumnOfBlock + 2) ? m_possibleAtPosition[firstRowOfBlock + 1][firstColumnOfBlock + 2] : 0,
                        isFree(firstRowOfBlock + 2, firstColumnOfBlock + 2) ? m_possibleAtPosition[firstRowOfBlock + 2][firstColumnOfBlock + 2] : 0});
}

bool Sudoku::fillHiddenSinglesInBlocks(bool& singleWasFilled) {
    for (unsigned short block = 0; block != 9; ++block) {
        const size_t       firstRowOfBlock    = DIV_THREE_TIMES_THREE[block];
        const size_t       firstColumnOfBlock = (3 * block) % 9;
        const NumberVector singles            = findSinglesInBlock(firstRowOfBlock, firstColumnOfBlock);
        for (unsigned short row = firstRowOfBlock; row != firstRowOfBlock + 3; ++row) {
            for (unsigned short column = firstColumnOfBlock; column != firstColumnOfBlock + 3; ++column) {
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
    }
    return true;
}

size_t Sudoku::numberOfNakedSingledCreated(unsigned short row, unsigned short column, unsigned short value) {
    unsigned short count = 0;
    for (unsigned short index = 0; index != 9; ++index) {
        count += (isFree(row, index) && m_possibleAtPosition[row][index].count() == 2 && m_possibleAtPosition[row][index].contains(value));
        count += (isFree(index, column) && m_possibleAtPosition[index][column].count() == 2 && m_possibleAtPosition[index][column].contains(value));
    }
    const unsigned short firstRowOfBlock    = DIV_THREE_TIMES_THREE[row];
    const unsigned short firstColumnOfBlock = DIV_THREE_TIMES_THREE[column];
    for (unsigned short r = firstRowOfBlock; r != firstRowOfBlock + 3; ++r) {
        for (unsigned short c = firstColumnOfBlock; c != firstColumnOfBlock + 3; ++c) {
            count += (isFree(r, c) && m_possibleAtPosition[r][c].count() == 2 && m_possibleAtPosition[r][c].contains(value));
        }
    }
    return count - 3 * (m_possibleAtPosition[row][column].count() == 2);
}

size_t Sudoku::numberOfNakedSingledCreated(unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    size_t result = 0;
    for (const auto number : m_possibleAtPosition[row][column].allEntries()) {
        result += numberOfNakedSingledCreated(row, column, number);
    }
    return result;
}
