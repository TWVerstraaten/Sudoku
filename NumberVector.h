//
// Created by pc on 21-09-21.
//

#ifndef H__NUMBERVECTOR_H
#define H__NUMBERVECTOR_H

#include <string>
#include <vector>

class NumberVector {

  public:
    NumberVector() = default;
    NumberVector(uint16_t value);

    void                                      add(uint8_t number);
    void                                      remove(uint8_t number);
    [[nodiscard]] bool                        is_empty() const;
    [[nodiscard]] bool                        contains(uint8_t number) const;
    [[nodiscard]] bool                        has_one_through_nine() const;
    [[nodiscard]] uint8_t                     count() const;
    [[nodiscard]] uint8_t                     smallest_number() const;
    [[nodiscard]] NumberVector                invert() const;
    [[nodiscard]] std::string                 to_string() const;
    [[nodiscard]] const std::vector<uint8_t>& all_entries() const;
    const NumberVector&                       operator|=(const NumberVector& other);
    const NumberVector&                       operator&=(const NumberVector& other);

    friend NumberVector operator|(NumberVector lhs, const NumberVector& rhs);
    friend NumberVector operator&(NumberVector lhs, const NumberVector& rhs);
    friend bool         operator==(const NumberVector& lhs, const NumberVector& rhs);

  private:
    uint16_t m_number_bits = 0;
};

#endif // H__NUMBERVECTOR_H
