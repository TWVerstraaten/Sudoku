//
// Created by pc on 21-09-21.
//

#include "NumberVector.h"

#include <cassert>
#include <sstream>

static size_t ADD_MASK[10]    = {0ul, 1ul << 1ul, 1ul << 2ul, 1ul << 3ul, 1ul << 4ul, 1ul << 5ul, 1ul << 6ul, 1ul << 7ul, 1ul << 8ul, 1ul << 9ul};
static size_t REMOVE_MASK[10] = {~ADD_MASK[0], ~ADD_MASK[1], ~ADD_MASK[2], ~ADD_MASK[3], ~ADD_MASK[4], ~ADD_MASK[5], ~ADD_MASK[6], ~ADD_MASK[7], ~ADD_MASK[8], ~ADD_MASK[9]};
static unsigned short COUNT_PRESENT[1ul << 10ul]    = {0};
static unsigned short COUNT_MISSING[1ul << 10ul]    = {0};
static unsigned short SMALLEST_MISSING[1ul << 10ul] = {0};
static unsigned short LARGEST_MISSING[1ul << 10ul]  = {0};

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
            if (!(i & (1 << bit))) {
                SMALLEST_MISSING[i] = bit;
                break;
            }
        }
    }
    for (size_t i = 0; i != 1ul << 10ul; ++i) {
        for (size_t bit = 9; bit >= 1; --bit) {
            if (!(i & (1 << bit))) {
                LARGEST_MISSING[i] = bit;
                break;
            }
        }
    }
}

NumberVector NumberVector::operator|=(const NumberVector& other) {
    m_numberBits |= other.m_numberBits;
    return *this;
}

void NumberVector::add(size_t number) {
    assert(number <= 9);
    m_numberBits |= ADD_MASK[number];
}

void NumberVector::remove(size_t number) {
    assert(number <= 9);
    assert(contains(number));
    m_numberBits &= REMOVE_MASK[number];
}

bool NumberVector::contains(size_t number) const {
    assert(number <= 9);
#ifdef NUMBER_VECTOR_USING_BITSET
    return m_numberBits[number];
#else
    return m_numberBits & ADD_MASK[number];
#endif
}

unsigned short NumberVector::count() const {
    return COUNT_PRESENT[m_numberBits];
}

unsigned short NumberVector::missing() const {
    return COUNT_MISSING[m_numberBits];
}

bool NumberVector::operator==(const NumberVector& other) const {
    return m_numberBits == other.m_numberBits;
}

bool NumberVector::hasOneThroughNine() const {
    return COUNT_MISSING[m_numberBits] == 0;
}

unsigned short NumberVector::smallestMissing() const {
    assert(not hasOneThroughNine());
    assert(1 <= SMALLEST_MISSING[m_numberBits] && SMALLEST_MISSING[m_numberBits] <= 9);
    return SMALLEST_MISSING[m_numberBits];
}

unsigned short NumberVector::largestMissing() const {
    assert(not hasOneThroughNine());
    assert(1 <= LARGEST_MISSING[m_numberBits] && LARGEST_MISSING[m_numberBits] <= 9);
    return LARGEST_MISSING[m_numberBits];
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
