//
// Created by pc on 21-09-21.
//

#include "Row.h"

#include <cassert>
#include <cctype>

#define BITS_PER_NUMBER 4ul
#define NUMBER_BIT_MASK 0xful

static size_t SHIFT_PER_COLUMN[9] = {
    BITS_PER_NUMBER * 0ul,
    BITS_PER_NUMBER * 1ul,
    BITS_PER_NUMBER * 2ul,
    BITS_PER_NUMBER * 3ul,
    BITS_PER_NUMBER * 4ul,
    BITS_PER_NUMBER * 5ul,
    BITS_PER_NUMBER * 6ul,
    BITS_PER_NUMBER * 7ul,
    BITS_PER_NUMBER * 8ul,
};

static size_t TOGGLE_OFF_MASK[9] = {~(NUMBER_BIT_MASK << SHIFT_PER_COLUMN[0]),
                                    ~(NUMBER_BIT_MASK << SHIFT_PER_COLUMN[1]),
                                    ~(NUMBER_BIT_MASK << SHIFT_PER_COLUMN[2]),
                                    ~(NUMBER_BIT_MASK << SHIFT_PER_COLUMN[3]),
                                    ~(NUMBER_BIT_MASK << SHIFT_PER_COLUMN[4]),
                                    ~(NUMBER_BIT_MASK << SHIFT_PER_COLUMN[5]),
                                    ~(NUMBER_BIT_MASK << SHIFT_PER_COLUMN[6]),
                                    ~(NUMBER_BIT_MASK << SHIFT_PER_COLUMN[7]),
                                    ~(NUMBER_BIT_MASK << SHIFT_PER_COLUMN[8])};

static size_t SET_MASK[9] = { NUMBER_BIT_MASK << SHIFT_PER_COLUMN[0],
                                     NUMBER_BIT_MASK << SHIFT_PER_COLUMN[1],
                                     NUMBER_BIT_MASK << SHIFT_PER_COLUMN[2],
                                     NUMBER_BIT_MASK << SHIFT_PER_COLUMN[3],
                                     NUMBER_BIT_MASK << SHIFT_PER_COLUMN[4],
                                     NUMBER_BIT_MASK << SHIFT_PER_COLUMN[5],
                                     NUMBER_BIT_MASK << SHIFT_PER_COLUMN[6],
                                     NUMBER_BIT_MASK << SHIFT_PER_COLUMN[7],
                                     NUMBER_BIT_MASK << SHIFT_PER_COLUMN[8]};

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

unsigned short Row::numberAt(unsigned short column) const {
    assert(column < 9);
    return (m_entries >> SHIFT_PER_COLUMN[column]) & NUMBER_BIT_MASK;
}

bool Row::numberAtPositionIsSet(unsigned short column) const {
    assert(column < 9);
    return m_entries & SET_MASK[column];
}

void Row::set(size_t column, size_t value) {
    assert(column < 9);
    m_entries &= TOGGLE_OFF_MASK[column];
    if (value != 0) {
        m_entries |= value << SHIFT_PER_COLUMN[column];
    }
}

bool Row::operator==(const Row& other) const {
    return m_entries == other.m_entries;
}
