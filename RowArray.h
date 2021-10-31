//
// Created by pc on 27-09-21.
//

#ifndef H__ROWARRAY_H
#define H__ROWARRAY_H

#include <array>

class RowArray {
  public:
    RowArray() = default;
    explicit RowArray(const std::string& string);

    void                  set(uint8_t column, uint8_t value);
    [[nodiscard]] uint8_t number_at(uint8_t column) const;
    [[nodiscard]] bool    position_is_free(uint8_t column) const;
    [[nodiscard]] bool    operator==(const RowArray& other) const;

  private:
    std::array<uint8_t, 9> m_entries{};
};

#endif // H__ROWARRAY_H
