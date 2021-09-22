//
// Created by pc on 21-09-21.
//

#ifndef _NUMBERVECTOR_H
#define _NUMBERVECTOR_H

//#define NUMBER_VECTOR_USING_BITSET

#include <limits>
#include <string>
#include <vector>

#ifdef NUMBER_VECTOR_USING_BITSET
#include <bitset>
#endif

class NumberVector {

  public:
    NumberVector() = default;

    NumberVector operator|=(const NumberVector& other);

    void set(size_t index);

    void unSet(size_t index);

    bool isSet(size_t index) const;

    unsigned short count() const;

    unsigned short missing() const;

    unsigned short firstMissing() const;

    bool hasOneThroughNine() const;

    bool operator==(const NumberVector& other) const;

  private:
#ifdef NUMBER_VECTOR_USING_BITSET
    std::bitset<10> m_numberBits;
#else
    int64_t m_numberBits = 0;
#endif
};

#endif //_NUMBERVECTOR_H
