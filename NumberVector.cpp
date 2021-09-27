//
// Created by pc on 21-09-21.
//

#include "NumberVector.h"

#include <array>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

static const size_t ADD_MASK[10]    = {0ul, 1ul << 1ul, 1ul << 2ul, 1ul << 3ul, 1ul << 4ul, 1ul << 5ul, 1ul << 6ul, 1ul << 7ul, 1ul << 8ul, 1ul << 9ul};
static const size_t REMOVE_MASK[10] = {~ADD_MASK[0], ~ADD_MASK[1], ~ADD_MASK[2], ~ADD_MASK[3], ~ADD_MASK[4], ~ADD_MASK[5], ~ADD_MASK[6], ~ADD_MASK[7], ~ADD_MASK[8], ~ADD_MASK[9]};
static uint8_t      COUNT_PRESENT[1ul << 10ul]                   = {0};
static uint8_t      COUNT_MISSING[1ul << 10ul]                   = {0};
static uint8_t      SMALLEST_NUMBER[1ul << 10ul]                 = {0};
static std::array<std::vector<uint8_t>, 1ul << 10ul> ALL_ENTRIES = {{}};

void NumberVector::INIT_ARRAYS() {
    for (size_t i = 0; i != 1ul << 10ul; ++i) {
        COUNT_PRESENT[i] = (i & 1) + COUNT_PRESENT[i >> 1];
    }
    for (size_t i = 0; i != 1ul << 10ul; ++i) {
        COUNT_PRESENT[i] -= (i & 1);
    }
    for (size_t i = 0; i != 1ul << 10ul; ++i) {
        COUNT_MISSING[i] = 9 - COUNT_PRESENT[i];
    }
    for (size_t i = 0; i != 1ul << 10ul; ++i) {
        for (size_t bit = 1; bit <= 9; ++bit) {
            if (i & (1 << bit)) {
                SMALLEST_NUMBER[i] = bit;
                break;
            }
        }
    }
    for (size_t i = 0; i != 1ul << 10ul; ++i) {
        for (size_t bit = 1; bit <= 9; ++bit) {
            if (i & (1 << bit)) {
                ALL_ENTRIES[i].emplace_back(bit);
            }
        }
    }
}

NumberVector::NumberVector(uint16_t numberBits) : m_numberBits(numberBits) {
}

NumberVector& NumberVector::operator|=(const NumberVector& other) {
    m_numberBits |= other.m_numberBits;
    return *this;
}

NumberVector& NumberVector::operator&=(const NumberVector& other) {
    m_numberBits &= other.m_numberBits;
    return *this;
}

NumberVector NumberVector::invert() const {
    NumberVector result;
    result.m_numberBits |= (~m_numberBits) & 0x3ff;
    return result;
}

void NumberVector::add(uint8_t number) {
    assert(number <= 9);
    m_numberBits = m_numberBits | ADD_MASK[number];
}

void NumberVector::remove(uint8_t number) {
    assert(number <= 9);
    m_numberBits &= REMOVE_MASK[number];
}

bool NumberVector::contains(uint8_t number) const {
    assert(number <= 9);
    return m_numberBits & ADD_MASK[number];
}

uint8_t NumberVector::count() const {
    return COUNT_PRESENT[m_numberBits];
}

bool NumberVector::hasOneThroughNine() const {
    return COUNT_MISSING[m_numberBits] == 0;
}

uint8_t NumberVector::smallestNumber() const {
    assert(not hasOneThroughNine());
    assert(1 <= SMALLEST_NUMBER[m_numberBits] && SMALLEST_NUMBER[m_numberBits] <= 9);
    return SMALLEST_NUMBER[m_numberBits];
}

std::string NumberVector::toString() const {
    std::stringstream ss;
    for (size_t i = 1; i != 10; ++i) {
        if (contains(i)) {
            ss << i << " ";
        }
    }
    return ss.str();
}

NumberVector operator|(NumberVector lhs, const NumberVector& rhs) {
    lhs |= rhs;
    return lhs;
}

NumberVector operator&(NumberVector lhs, const NumberVector& rhs) {
    lhs.m_numberBits &= rhs.m_numberBits;
    return lhs;
}

const std::vector<uint8_t>& NumberVector::allEntries() const {
    return ALL_ENTRIES[m_numberBits];
}

bool NumberVector::isEmpty() const {
    return COUNT_PRESENT[m_numberBits] == 0;
}
