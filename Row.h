//
// Created by pc on 21-09-21.
//

#ifndef _ROW_H
#define _ROW_H

#include "NumberVector.h"

#include <cstddef>
#include <set>
#include <string>
#include <vector>

class Row {

  public:
    Row() = default;

    explicit Row(const std::string& string);

    unsigned short numberAt(unsigned short index) const;

    const NumberVector& numbersInRow() const;

    void set(size_t index, size_t value);

    bool operator==(const Row& other) const;

  private:
    size_t       m_concatenated = 0ul;
    NumberVector m_numbersInRow;
};

#endif //_ROW_H
