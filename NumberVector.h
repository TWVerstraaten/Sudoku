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

    void add(size_t number);

    void remove(size_t number);

    bool contains(size_t number) const;

    unsigned short count() const;

    unsigned short missing() const;

    unsigned short smallestMissing() const;

    unsigned short largestMissing() const;

    bool hasOneThroughNine() const;

    bool operator==(const NumberVector& other) const;

    std::string toString() const;

    static void INIT_ARRAYS();

  private:
#ifdef NUMBER_VECTOR_USING_BITSET
    std::bitset<10> m_numberBits;
#else
    size_t m_numberBits = 0;
#endif
};

#endif //_NUMBERVECTOR_H
