//
// Created by pc on 27-09-21.
//

#include "RowArray.h"

#include <cassert>

RowArray::RowArray(const std::string& string) {
    assert(string.length() == 9);
    size_t index = 0;
    for (auto c : string) {
        if (c != '.') {
            assert(isdigit(c));
            m_entries[index] = c - '0';
        }
        ++index;
    }
}

uint8_t RowArray::number_at(uint8_t column) const {
    return m_entries[column];
}

void RowArray::set(uint8_t column, uint8_t value) {
    m_entries[column] = value;
}

bool RowArray::operator==(const RowArray& other) const {
    return other.m_entries == m_entries;
}

bool RowArray::position_is_free(uint8_t column) const {
    return m_entries[column] == 0;
}
