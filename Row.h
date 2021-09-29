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

    void                         set(size_t column, size_t value);
    [[nodiscard]] unsigned short numberAt(unsigned short column) const;
    [[nodiscard]] bool           operator==(const Row& other) const;
    [[nodiscard]] bool           positionIsFree(unsigned short column) const;

  private:
    size_t m_entries = 0ul;
};

#endif //_ROW_H
