//
// Created by pc on 21-09-21.
//

#include "Sudoku.h"

#include <cassert>
#include <iostream>
#include <limits>
#include <memory>
#include <set>
#include <sstream>

Sudoku::Sudoku(const std::string& string) {
    assert(string.length() == 81);
    for (unsigned short i = 0; i != 9; ++i) {
        m_rows[i] = Row(string.substr(9 * i, 9));
    }
    for (size_t row = 0; row != 9; ++row) {
        for (size_t column = 0; column != 9; ++column) {
            m_numbersInColumns[column].add(m_rows[row].numberAt(column));
            m_numbersInBlocks[indicesToBlock(row, column)].add(m_rows[row].numberAt(column));
            m_numbersInRows[row].add(m_rows[row].numberAt(column));
        }
    }
    for (size_t row = 0; row != 9; ++row) {
        for (size_t column = 0; column != 9; ++column) {
            m_possibleAtPosition[row][column] = ruledOutAt(row, column).invert();
            m_sortedPossible[m_possibleAtPosition[row][column].count()].push_back({row, column});
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

NumberVector Sudoku::numbersInColumn(unsigned short column) const {
    return m_numbersInColumns[column];
}

NumberVector Sudoku::numbersInRow(unsigned short row) const {
    return m_numbersInRows[row];
}

NumberVector Sudoku::numbersInBlock(unsigned short block) const {
    return m_numbersInBlocks[block];
}

unsigned short Sudoku::indicesToBlock(unsigned short row, unsigned short column) {
    assert(row < 9);
    assert(column < 9);
    return (row / 3) * 3 + (column / 3);
}

void Sudoku::set(unsigned short row, unsigned short column, unsigned short value) {
    assert(row < 9);
    m_numbersInColumns[column].add(value);
    m_numbersInBlocks[indicesToBlock(row, column)].add(value);
    m_numbersInRows[row].add(value);

    auto& m_possibleAtRow = m_possibleAtPosition[row];
    for (unsigned short index = 0; index != 9; ++index) {
        m_possibleAtRow[index].remove(value);
        m_possibleAtPosition[index][column].remove(value);
    }
    unsigned short rowStart    = 3 * (row / 3);
    unsigned short columnStart = 3 * (column / 3);
    for (unsigned short i = rowStart; i != rowStart + 3; i++) {
        for (unsigned short j = columnStart; j != columnStart + 3; ++j) {
            m_possibleAtPosition[i][j].remove(value);
        }
    }
    m_rows[row].set(column, value);
}

bool Sudoku::isFree(unsigned short row, unsigned short column) const {
    assert(row < 9);
    assert(column < 9);
    return not m_rows[row].numberAtPositionIsSet(column);
}

bool Sudoku::solve() {
    unsigned short minMissing = std::numeric_limits<unsigned short>::max();
    unsigned short minRow;
    unsigned short minColumn;
    NumberVector   numberVector;
    findNextBestMove(minRow, minColumn, numberVector, minMissing);
    switch (minMissing) {
        case 0: // There is an empty spot where no numbers can fit
            return false;
        case 1: // There is an empty spot where just one can fit
            return solveWithSingleSubstitution(minRow, minColumn, numberVector.smallestMissing());
        case std::numeric_limits<unsigned short>::max(): // No spot found with any missing ==> solved!
            return true;
        default: // Spot found with > 1  missing numbers, try them
            return solveWithSubstitutionsUsingCopy(minRow, minColumn, numberVector);
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

// NumberVector Sudoku::findSinglesInRow(unsigned short row) {
//     NumberVector tooMany;
//     NumberVector result;
//     const auto&  possibleInRow = m_possibleAtPosition[row];
//     for (unsigned short column = 0; column != 9; ++column) {
//         if (isFree(row, column)) {
//             tooMany |= (possibleInRow[column] & result);
//             result |= possibleInRow[column];
//         }
//     }
//     return result & (tooMany.invert());
// }
//
// void Sudoku::fillHiddenSingles() {
//     for (unsigned short row = 0; row != 9; ++row) {
//         NumberVector singles = findSinglesInRow(row);
//         for (unsigned short column = 0; column != 9; ++column) {
//             if (isFree(row, column)) {
//                 for (unsigned short pos = 1; pos != 10; ++pos) {
//                     if (singles.contains(pos) && m_possibleAtPosition[row][column].contains(pos)) {
//                         set(row, column, pos);
//                         break;
//                     }
//                 }
//             }
//         }
//     }
//     for (unsigned short column = 0; column != 9; ++column) {
//         NumberVector singles = findSingles({isFree(0, column) ? m_possibleAtPosition[0][column] : 0,
//                                             isFree(1, column) ? m_possibleAtPosition[1][column] : 0,
//                                             isFree(2, column) ? m_possibleAtPosition[2][column] : 0,
//                                             isFree(3, column) ? m_possibleAtPosition[3][column] : 0,
//                                             isFree(4, column) ? m_possibleAtPosition[4][column] : 0,
//                                             isFree(5, column) ? m_possibleAtPosition[5][column] : 0,
//                                             isFree(6, column) ? m_possibleAtPosition[6][column] : 0,
//                                             isFree(7, column) ? m_possibleAtPosition[7][column] : 0,
//                                             isFree(8, column) ? m_possibleAtPosition[8][column] : 0});
//         for (unsigned short row = 0; row != 9; ++row) {
//             if (isFree(row, column)) {
//                 for (unsigned short pos = 1; pos != 10; ++pos) {
//                     if (singles.contains(pos) && m_possibleAtPosition[row][column].contains(pos)) {
//                         set(row, column, pos);
//                         break;
//                     }
//                 }
//             }
//         }
//     }
// }

size_t Sudoku::countSolutions() {
    //    fillHiddenSingles();
    bool           singlesWereFilled = false;
    unsigned short minimumPossible   = std::numeric_limits<unsigned short>::max();
    unsigned short minRow            = std::numeric_limits<unsigned short>::max();
    unsigned short minColumn         = std::numeric_limits<unsigned short>::max();
    for (size_t row = 0; row != 9; ++row) {
        for (size_t column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                const unsigned short count = m_possibleAtPosition[row][column].count();
                if (count == 0) {
                    return 0;
                } else if (count == 1) {
                    singlesWereFilled = true;
                    set(row, column, m_possibleAtPosition[row][column].invert().smallestMissing());
                } else if (not singlesWereFilled) {
                    if (count < minimumPossible) {
                        minimumPossible = count;
                        minRow          = row;
                        minColumn       = column;
                    }
                }
            }
        }
    }
    if (singlesWereFilled) {
        return countSolutions();
    } else if (minimumPossible == std::numeric_limits<unsigned short>::max()) {
        return 1;
    } else {
        return countWithSubstitutionsUsingCopy(minRow, minColumn);
    }
}

bool Sudoku::solveWithSubstitutionsUsingCopy(unsigned short row, unsigned short column, const NumberVector& numberVector) {
    assert(isFree(row, column));
    for (unsigned short i = 1; i < 10; ++i) {
        if (not numberVector.contains(i)) {
            Sudoku copy = *this;
            copy.set(row, column, i);
            if (copy.solve()) {
                m_rows = copy.m_rows;
                return true;
            }
        }
    }
    return false;
}

size_t Sudoku::countWithSubstitutionsUsingCopy(unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    size_t     count           = 0;
    const auto possibleEntries = m_possibleAtPosition[row][column];
    for (unsigned short i = 1; i != 10; ++i) {
        if (possibleEntries.contains(i)) {
            Sudoku copy = *this;
            copy.set(row, column, i);
            count += copy.countSolutions();
        }
    }
    return count;
}

NumberVector Sudoku::ruledOutAt(unsigned short row, unsigned short column) const {
    return numbersInRow(row) | numbersInColumn(column) | numbersInBlock(indicesToBlock(row, column));
}

bool Sudoku::operator==(const Sudoku& other) {
    return m_rows == other.m_rows;
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

bool Sudoku::solveWithSingleSubstitution(unsigned short row, unsigned short column, unsigned short onlyPossible) {
    assert(isFree(row, column));
    Sudoku s = *this;
    s.set(row, column, onlyPossible);
    return s.solve();
}

void Sudoku::findNextBestMove(unsigned short& minRow, unsigned short& minColumn, NumberVector& numberVector, unsigned short& possibleCount) {
    possibleCount = std::numeric_limits<unsigned short>::max();
    for (unsigned short row = 0; row != 9; ++row) {
        for (unsigned short column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                const NumberVector v = m_possibleAtPosition[row][column].invert();
                if (v.missing() < possibleCount) {
                    possibleCount = v.missing();
                    minRow        = row;
                    minColumn     = column;
                    numberVector  = v;
                    if (possibleCount == 1) {
                        return;
                    }
                }
            }
        }
    }
}

bool Sudoku::canBePlaced(unsigned short row, unsigned short column, unsigned short value) {
    return not(m_numbersInRows[row].contains(value)) && (not m_numbersInColumns[column].contains(value)) && (not m_numbersInBlocks[indicesToBlock(row, column)].contains(value));
}
