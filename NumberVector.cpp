//
// Created by pc on 21-09-21.
//

#include "NumberVector.h"

#include <cassert>

NumberVector NumberVector::operator|=(const NumberVector& other) {
    m_numberBits |= other.m_numberBits;
    return *this;
}

void NumberVector::set(size_t index) {
    assert(index <= 9);
    if (index == 0) {
        return;
    }
    m_numberBits |= (1 << index);
}

void NumberVector::unSet(size_t index) {
    assert(index <= 9);
    assert(isSet(index));
    m_numberBits &= ~(1 << index);
}

bool NumberVector::isSet(size_t index) const {
    assert(index <= 9);
    return m_numberBits & (1 << index);
}

unsigned short NumberVector::count() const {
    unsigned short c = 0;
    for (unsigned short i = 1; i != 10; ++i) {
        if (isSet(i)) {
            ++c;
        }
    }
    return c;
}

unsigned short NumberVector::missing() const {
    return 9 - count();
}

bool NumberVector::operator==(const NumberVector& other) const {
    return m_numberBits == other.m_numberBits;
}

bool NumberVector::hasOneToNine() const {
    if (m_numberBits == 0x1ff) {
        assert(count() == 9);
        return true;
    }
    return false;
}

unsigned short NumberVector::firstMissing() const {
    assert(missing() > 0);
    for (unsigned short i = 1; i != 10; ++i) {
        if (not isSet(i)) {
            return i;
        }
    }
    assert(false);
}
