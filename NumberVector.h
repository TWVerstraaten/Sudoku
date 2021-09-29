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

    explicit NumberVector(uint16_t numberBits);

    void                                      add(uint8_t number);
    void                                      remove(uint8_t number);
    const NumberVector&                       operator|=(const NumberVector& other);
    const NumberVector&                       operator&=(const NumberVector& other);
    [[nodiscard]] bool                        isEmpty() const;
    [[nodiscard]] bool                        contains(uint8_t number) const;
    [[nodiscard]] bool                        hasOneThroughNine() const;
    [[nodiscard]] uint8_t                     count() const;
    [[nodiscard]] uint8_t                     smallestNumber() const;
    [[nodiscard]] NumberVector                invert() const;
    [[nodiscard]] std::string                 toString() const;
    [[nodiscard]] const std::vector<uint8_t>& allEntries() const;

    friend NumberVector operator|(NumberVector lhs, const NumberVector& rhs);
    friend NumberVector operator&(NumberVector lhs, const NumberVector& rhs);
    friend bool         operator==(const NumberVector& lhs, const NumberVector& rhs);

  private:
    uint16_t m_numberBits = 0;
};

#endif //_NUMBERVECTOR_H
