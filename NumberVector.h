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

    NumberVector(unsigned short numberBits);

    NumberVector& operator|=(const NumberVector& other);

    NumberVector& operator&=(const NumberVector& other);

    bool isEmpty() const;

    friend NumberVector operator|(NumberVector lhs, const NumberVector& rhs);

    friend NumberVector operator&(NumberVector lhs, const NumberVector& rhs);

    NumberVector invert() const;

    void add(size_t number);

    void remove(size_t number);

    bool contains(size_t number) const;

    unsigned short count() const;

    unsigned short smallestNumber() const;

    bool hasOneThroughNine() const;

    std::string toString() const;

    static void INIT_ARRAYS();

    const std::vector<unsigned short>& allEntries() const;

  private:
    unsigned short m_numberBits = 0;
};

#endif //_NUMBERVECTOR_H
