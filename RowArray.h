//
// Created by pc on 27-09-21.
//

#ifndef _ROWARRAY_H
#define _ROWARRAY_H

#include <array>

class RowArray {
  public:
    RowArray() = default;

    explicit RowArray(const std::string& string);

    uint8_t  numberAt(uint8_t  column) const;

    void set(uint8_t  column, uint8_t  value);

    bool operator==(const RowArray& other) const;

    bool positionIsFree(uint8_t  column) const;

  private:
    std::array<uint8_t , 9> m_entries{};
};

#endif //_ROWARRAY_H