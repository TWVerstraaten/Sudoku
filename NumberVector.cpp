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

NumberVector::NumberVector(uint16_t value) : m_number_bits(value) {
}

const NumberVector& NumberVector::operator|=(const NumberVector& other) {
    m_number_bits |= other.m_number_bits;
    return *this;
}

const NumberVector& NumberVector::operator&=(const NumberVector& other) {
    m_number_bits &= other.m_number_bits;
    return *this;
}

NumberVector NumberVector::invert() const {
    return g_inverted[m_number_bits];
}

void NumberVector::add(uint8_t number) {
    assert(number <= 9);
    m_number_bits |= g_add_mask[number];
}

void NumberVector::remove(uint8_t number) {
    assert(number <= 9);
    m_number_bits &= g_remove_mask[number];
}

bool NumberVector::contains(uint8_t number) const {
    assert(number <= 9);
    return m_number_bits & g_add_mask[number];
}

uint8_t NumberVector::count() const {
    return g_count_present[m_number_bits];
}

bool NumberVector::has_one_through_nine() const {
    return g_count_missing[m_number_bits] == 0;
}

uint8_t NumberVector::smallest_number() const {
    assert(not has_one_through_nine());
    assert(1 <= g_smallest_number[m_number_bits]);
    assert(g_smallest_number[m_number_bits] <= 9);
    return g_smallest_number[m_number_bits];
}

std::string NumberVector::to_string() const {
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
    lhs.m_number_bits &= rhs.m_number_bits;
    return lhs;
}

const std::vector<uint8_t>& NumberVector::all_entries() const {
    return g_all_entries[m_number_bits];
}

bool NumberVector::is_empty() const {
    return g_count_present[m_number_bits] == 0;
}
bool operator==(const NumberVector& lhs, const NumberVector& rhs) {
    return lhs.m_number_bits == rhs.m_number_bits;
}
