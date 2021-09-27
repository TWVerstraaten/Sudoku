//
// Created by pc on 21-09-21.
//

#ifndef _NUMBERVECTOR_H
#define _NUMBERVECTOR_H

#include <limits>
#include <string>
#include <vector>

class NumberVector {

  public:
    NumberVector() = default;

    NumberVector(uint16_t numberBits);

    NumberVector& operator|=(const NumberVector& other);

    NumberVector& operator&=(const NumberVector& other);

    bool isEmpty() const;

    friend NumberVector operator|(NumberVector lhs, const NumberVector& rhs);

    friend NumberVector operator&(NumberVector lhs, const NumberVector& rhs);

    NumberVector invert() const;

    void add(unsigned char number);

    void remove(unsigned char number);

    bool contains(unsigned char number) const;

    unsigned char count() const;

    unsigned char smallestNumber() const;

    bool hasOneThroughNine() const;

    std::string toString() const;

    static void INIT_ARRAYS();

    const std::vector<unsigned char>& allEntries() const;

  private:
    uint16_t m_numberBits = 0;
};

#endif //_NUMBERVECTOR_H
