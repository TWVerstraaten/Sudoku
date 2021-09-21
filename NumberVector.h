//
// Created by pc on 21-09-21.
//

#ifndef _NUMBERVECTOR_H
#define _NUMBERVECTOR_H

#include <string>
#include <vector>

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

    bool hasOneToNine() const;

    bool operator==(const NumberVector& other) const;

  private:
    unsigned short m_numberBits = 0;
};

#endif //_NUMBERVECTOR_H
