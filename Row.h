//
// Created by pc on 21-09-21.
//

#ifndef _ROW_H
#define _ROW_H

#include "NumberVector.h"

#include <string>

class Row {

  public:
    Row() = default;

    explicit Row(const std::string& string);

    unsigned char numberAt(unsigned char column) const;

    bool numberAtPositionIsSet(unsigned char column) const;

    void set(size_t column, size_t value);

    bool operator==(const Row& other) const;

  private:
    size_t m_entries = 0ul;
};

#endif //_ROW_H
