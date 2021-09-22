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
    for (size_t column = 0; column != 9; ++column) {
        for (size_t row = 0; row != 9; ++row) {
            m_numbersInColumns[column].set(m_rows[row].numberAt(column));
            m_numbersInBlocks[indicesToBlock(row, column)].set(m_rows[row].numberAt(column));
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
    if (m_rows[row].numberAt(column) != 0) {
        m_numbersInColumns[column].unSet(m_rows[row].numberAt(column));
        m_numbersInBlocks[indicesToBlock(row, column)].unSet(m_rows[row].numberAt(column));
    }
    m_numbersInColumns[column].set(value);
    m_numbersInBlocks[indicesToBlock(row, column)].set(value);
    m_rows[row].set(column, value);
}

bool Sudoku::isFree(unsigned short row, unsigned short column) const {
    assert(row < 9);
    assert(column < 9);
    return m_rows[row].numberAt(column) == 0;
}

size_t Sudoku::solve() {
    unsigned short minMissing = std::numeric_limits<unsigned short>::max();
    unsigned short minRow;
    unsigned short minColumn;
    NumberVector   numberVector;
    for (unsigned short row = 0; row != 9; ++row) {
        for (unsigned short column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                const NumberVector v = ruledOutAt(row, column);
                if (v.hasOneThroughNine()) {
                    return 0;
                } else if (v.missing() < minMissing) {
                    minMissing   = v.missing();
                    minRow       = row;
                    minColumn    = column;
                    numberVector = v;
                    //                    goto label;
                }
            }
        }
    }
    // label:
    if (minMissing == std::numeric_limits<unsigned short>::max()) {
        return 1ul;
    } else if (minMissing == 1) {
        return trySingleAtPosition(numberVector.firstMissing(), minRow, minColumn);
    } else {
        return tryPossibilitiesAtPosition(numberVector, minRow, minColumn);
    }
}

bool Sudoku::tryPossibilitiesAtPositionUsingCopy(const NumberVector& numberVector, unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    for (unsigned short i = 1; i != 10; ++i) {
        if (not numberVector.isSet(i)) {
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

bool Sudoku::tryPossibilitiesAtPosition(const NumberVector& numberVector, unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    size_t count = 0;
    for (unsigned short i = 1; i != 10; ++i) {
        if (not numberVector.isSet(i)) {
            set(row, column, i);
            count += solve();
        }
    }
    set(row, column, 0);
    return count;
}

bool Sudoku::trySingleAtPosition(unsigned short onlyPossible, unsigned short row, unsigned short column) {
    assert(isFree(row, column));
    set(row, column, onlyPossible);
    size_t count = solve();
    set(row, column, 0);
    return count;
}

NumberVector Sudoku::ruledOutAt(unsigned short row, unsigned short column) const {
    assert(isFree(row, column));
    NumberVector result = m_rows[row].numbersInRow();
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
            return Sudoku{"...4.8..."
                          ".6..7..1."
                          "7.2.9.5.4"
                          ".9.7.4.3."
                          "..7.5.8.."
                          ".8.9.6.5."
                          "9.4.1.7.8"
                          ".7..6..4."
                          "...2.7..."};
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
            return Sudoku{"123456789"
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."};

        default:
            exit(1);
    }
}
