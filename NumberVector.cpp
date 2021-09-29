//
// Created by pc on 21-09-21.
//

#include "NumberVector.h"

#include "NumberVectorStaticArrays.h"

#include <array>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

NumberVector::NumberVector(uint16_t numberBits) : m_numberBits(numberBits) {
}

const NumberVector& NumberVector::operator|=(const NumberVector& other) {
    m_numberBits |= other.m_numberBits;
    return *this;
}

const NumberVector& NumberVector::operator&=(const NumberVector& other) {
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

bool operator==(const NumberVector& lhs, const NumberVector& rhs) {
    return lhs.m_numberBits == rhs.m_numberBits;
}
