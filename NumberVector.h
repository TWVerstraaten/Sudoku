//
// Created by pc on 21-09-21.
//

#ifndef _NUMBERVECTOR_H
#define _NUMBERVECTOR_H

//#define NUMBER_VECTOR_USING_BITSET

#include <limits>
#include <string>
#include <vector>

class NumberVector {

  public:
    NumberVector() = default;

    NumberVector(unsigned short numberBits);

    NumberVector& operator|=(const NumberVector& other);

    friend NumberVector operator|(NumberVector lhs, const NumberVector& rhs);

    friend NumberVector operator&(NumberVector lhs, const NumberVector& rhs);

    NumberVector invert() const;

    void add(size_t number);

    void remove(size_t number);

    bool contains(size_t number) const;

    unsigned char count() const;

    unsigned char missing() const;

    unsigned char smallestNumber() const;

    bool hasOneThroughNine() const;

    bool operator==(const NumberVector& other) const;

    std::string toString() const;

    static void INIT_ARRAYS();

  private:
    unsigned short m_numberBits = 0;
};

#endif //_NUMBERVECTOR_H
