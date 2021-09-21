//
// Created by pc on 21-09-21.
//

#ifndef _SUDOKU_H
#define _SUDOKU_H

#include "NumberVector.h"
#include "Row.h"

#include <array>
#include <vector>

class Sudoku {

  public:
    explicit Sudoku(const std::string& string);

    Sudoku(const Sudoku& other) = default;

    static Sudoku preset(size_t index);

    std::string toString() const;

    bool solve();

    bool operator==(const Sudoku& other);

  private:
    bool tryPossibilitiesAtPosition(const NumberVector& numberVector, unsigned short row, unsigned short column);

    bool trySingleAtPosition(unsigned short onlyPossible, unsigned short row, unsigned short column);

    void set(unsigned short row, unsigned short column, unsigned short value);

    bool isFree(unsigned short row, unsigned short column) const;

    NumberVector ruledOutAt(unsigned short row, unsigned short column) const;

    static unsigned short indicesToBlock(unsigned short row, unsigned short column);

    NumberVector numbersInColumn(unsigned short index) const;

    NumberVector numbersInBlock(unsigned short index) const;

    std::array<Row, 9> m_rows{};
};

#endif //_SUDOKU_H
