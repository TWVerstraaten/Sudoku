//
// Created by pc on 21-09-21.
//

#ifndef H__ROW_H
#define H__ROW_H

#include "NumberVector.h"

#include <string>

class Row {

  public:
    Row() = default;

    explicit Row(const std::string& string);

    void                         set(size_t column, size_t value);
    [[nodiscard]] unsigned short number_at(unsigned short column) const;
    [[nodiscard]] bool           position_is_free(unsigned short column) const;
    [[nodiscard]] bool           operator==(const Row& other) const;

  private:
    size_t m_entries = 0ul;
};

#endif // H__ROW_H
