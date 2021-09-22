//
// Created by pc on 21-09-21.
//

#include "Row.h"

#include <cassert>
#include <cctype>

#define BITS_PER_NUMBER 4ul
#define NUMBER_BIT_MASK 0xful

Row::Row(const std::string& string) : m_entries(0ul) {
    assert(string.length() == 9);
    size_t multiplier = 1ul;
    for (auto c : string) {
        if (c != '.') {
            assert(isdigit(c));
            m_entries |= multiplier * (c - '0');
        }
        multiplier <<= BITS_PER_NUMBER;
    }
}

unsigned short Row::numberAt(unsigned short index) const {
    assert(index < 9);
    return (m_entries >> (BITS_PER_NUMBER * index)) & NUMBER_BIT_MASK;
}

void Row::set(size_t index, size_t value) {
    assert(index < 9);
    m_entries &= ~(NUMBER_BIT_MASK << (BITS_PER_NUMBER * index));
    if (value != 0) {
        m_entries |= value << (BITS_PER_NUMBER * index);
    }
}

bool Row::operator==(const Row& other) const {
    return m_entries == other.m_entries;
}
