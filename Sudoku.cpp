//
// Created by pc on 21-09-21.
//

#include "Sudoku.h"

#include <cassert>
#include <iostream>
#include <limits>
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
    const unsigned short current = m_rows[row].numberAt(column);
    if (current != 0) {
        m_numbersInColumns[column].remove(current);
        m_numbersInBlocks[indicesToBlock(row, column)].remove(current);
        m_numbersInRows[row].remove(current);
    }
    m_numbersInColumns[column].add(value);
    m_numbersInBlocks[indicesToBlock(row, column)].add(value);
    m_numbersInRows[row].add(value);

    m_rows[row].set(column, value);
}

bool Sudoku::isFree(unsigned short row, unsigned short column) const {
    assert(row < 9);
    assert(column < 9);
    return m_rows[row].numberAt(column) == 0;
}

size_t Sudoku::countSolutions() {
    unsigned short minMissing = std::numeric_limits<unsigned short>::max();
    unsigned short minRow;
    unsigned short minColumn;
    NumberVector   numberVector;
    findNextBestMove(minRow, minColumn, numberVector, minMissing);
    switch (minMissing) {
        case 0: // There is an empty spot where no numbers can fit
            return 0;
        case 1: // There is an empty spot where just one can fit
            return countWithSingleSubstitution(numberVector.smallestMissing(), minRow, minColumn);
        case std::numeric_limits<unsigned short>::max(): // No spot found with any missing ==> solved!
            return 1ul;
        default: // Spot found with > 1  missing numbers, try them
            return countWithSubstitutionsUsingCopy(numberVector, minRow, minColumn);
    }
}

void Sudoku::fillHiddenSingles() {
}

size_t Sudoku::countWithSubstitutionsUsingCopy(const NumberVector& numberVector, unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    size_t count = 0;
    for (unsigned short i = 1; i < 10; ++i) {
        if (not numberVector.contains(i)) {
            Sudoku s = *this;
            s.set(row, column, i);
            count += s.countSolutions();
        }
    }
    return count;
}

size_t Sudoku::countWithSubstitutions(const NumberVector& numberVector, unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    size_t count = 0;
    for (unsigned short i = 1; i != 10; ++i) {
        if (not numberVector.contains(i)) {
            set(row, column, i);
            count += countSolutions();
        }
    }
    set(row, column, 0);
    return count;
}

size_t Sudoku::countWithSingleSubstitution(unsigned short onlyPossible, unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    set(row, column, onlyPossible);
    size_t count = countSolutions();
    set(row, column, 0);
    return count;
}

NumberVector Sudoku::ruledOutAt(unsigned short row, unsigned short column) const {
    assert(isFree(row, column));
    NumberVector result = numbersInRow(row);
    result |= numbersInColumn(column);
    result |= numbersInBlock(indicesToBlock(row, column));
    return result;
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
            return solveWithSingleSubstitution(numberVector.smallestMissing(), minRow, minColumn);
        case std::numeric_limits<unsigned short>::max(): // No spot found with any missing ==> solved!
            return true;
        default: // Spot found with > 1  missing numbers, try them
            return solveWithSubstitutions(numberVector, minRow, minColumn);
    }
}

bool Sudoku::solveWithSubstitutionsUsingCopy(const NumberVector& numberVector, unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    for (unsigned short i = 1; i < 10; ++i) {
        if (not numberVector.contains(i)) {
            Sudoku s = *this;
            s.set(row, column, i);
            if (s.solve()) {
                m_rows = s.m_rows;
                return true;
            }
        }
    }
    return false;
}

bool Sudoku::solveWithSubstitutions(const NumberVector& numberVector, unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    for (unsigned short i = 1; i != 10; ++i) {
        if (not numberVector.contains(i)) {
            set(row, column, i);
            if (solve()) {
                return true;
            }
        }
    }
    set(row, column, 0);
    return false;
}

bool Sudoku::solveWithSingleSubstitution(unsigned short onlyPossible, unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    set(row, column, onlyPossible);
    if (solve()) {
        return true;
    }
    set(row, column, 0);
    return false;
}

void Sudoku::findNextBestMove(unsigned short& minRow, unsigned short& minColumn, NumberVector& numberVector, unsigned short& minMissing) {
    minMissing = std::numeric_limits<unsigned short>::max();
    for (unsigned short row = 0; row != 9; ++row) {
        for (unsigned short column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                const NumberVector v = ruledOutAt(row, column);
                if (v.hasOneThroughNine()) {
                    minMissing = 0;
                    return;
                } else if (v.missing() < minMissing) {
                    minMissing   = v.missing();
                    minRow       = row;
                    minColumn    = column;
                    numberVector = v;
                    if (minMissing == 1) {
                        return;
                    }
                }
            }
        }
    }
}
