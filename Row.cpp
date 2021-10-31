//
// Created by pc on 21-09-21.
//

#include "Row.h"

#include <cassert>

#define BITS_PER_NUMBER 4ul
#define NUMBER_BIT_MASK 0xful

static size_t g_shift_per_column[9] = {
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
static size_t g_toggle_off_mask[9] = {~(NUMBER_BIT_MASK << g_shift_per_column[0]),
                                    ~(NUMBER_BIT_MASK << g_shift_per_column[1]),
                                    ~(NUMBER_BIT_MASK << g_shift_per_column[2]),
                                    ~(NUMBER_BIT_MASK << g_shift_per_column[3]),
                                    ~(NUMBER_BIT_MASK << g_shift_per_column[4]),
                                    ~(NUMBER_BIT_MASK << g_shift_per_column[5]),
                                    ~(NUMBER_BIT_MASK << g_shift_per_column[6]),
                                    ~(NUMBER_BIT_MASK << g_shift_per_column[7]),
                                    ~(NUMBER_BIT_MASK << g_shift_per_column[8])};
static size_t g_set_mask[9]        = {NUMBER_BIT_MASK << g_shift_per_column[0],
                             NUMBER_BIT_MASK << g_shift_per_column[1],
                             NUMBER_BIT_MASK << g_shift_per_column[2],
                             NUMBER_BIT_MASK << g_shift_per_column[3],
                             NUMBER_BIT_MASK << g_shift_per_column[4],
                             NUMBER_BIT_MASK << g_shift_per_column[5],
                             NUMBER_BIT_MASK << g_shift_per_column[6],
                             NUMBER_BIT_MASK << g_shift_per_column[7],
                             NUMBER_BIT_MASK << g_shift_per_column[8]};

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

unsigned short Row::number_at(unsigned short column) const {
    assert(column < 9);
    return (m_entries >> g_shift_per_column[column]) & NUMBER_BIT_MASK;
}

void Row::set(size_t column, size_t value) {
    assert(column < 9);
    m_entries &= g_toggle_off_mask[column];
    if (value != 0) {
        m_entries |= value << g_shift_per_column[column];
    }
}

bool Row::operator==(const Row& other) const {
    return m_entries == other.m_entries;
}

bool Row::position_is_free(unsigned short column) const {
    return not(m_entries & (g_set_mask[column]));
}
