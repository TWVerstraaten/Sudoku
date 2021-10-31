//
// Created by pc on 21-09-21.
//

#ifndef H__SUDOKU_H
#define H__SUDOKU_H

#include "NumberVector.h"
#include "RowArray.h"

#include <array>
#include <memory>

class Sudoku {

public:
    explicit Sudoku(const std::string &string);

    Sudoku(const Sudoku &other) = default;

    Sudoku &operator=(const Sudoku &other) = default;

    [[nodiscard]] static Sudoku preset(size_t index);

    [[nodiscard]] bool solve();

    [[nodiscard]] bool is_consistent() const;

    [[nodiscard]] bool is_solved() const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] std::shared_ptr<Sudoku> solution() const;

private:
    [[nodiscard]] bool deterministic_inferences();

    [[nodiscard]] bool counting_check() const;

    [[nodiscard]] bool row_counting_check(uint8_t row) const;

    [[nodiscard]] bool column_counting_check(uint8_t column) const;

    [[nodiscard]] bool block_counting_check(uint8_t block) const;

    [[nodiscard]] bool set(uint8_t row, uint8_t column, uint8_t value);

    [[nodiscard]] bool has_possibilities(uint8_t row, uint8_t column);

    [[nodiscard]] bool try_by_substituting(uint8_t row, uint8_t column);

    [[nodiscard]] bool is_free(uint8_t row, uint8_t column) const;

    [[nodiscard]] bool fill_all_hidden_singles(bool &single_was_filled);

    [[nodiscard]] bool fill_naked_singles(bool &single_was_filled);

    [[nodiscard]] bool fill_hidden_singles_in_rows(bool &single_was_filled);

    [[nodiscard]] bool fill_hidden_singles_in_columns(bool &single_was_filled);

    [[nodiscard]] bool fill_hidden_singles_in_blocks(bool &single_was_filled);

    [[nodiscard]] bool find_pointing_sets(bool &was_updated);

    [[nodiscard]] bool remove_if_present(uint8_t row, uint8_t column, uint8_t value, bool &was_removed);

    [[nodiscard]] float get_reduction_score(uint8_t row, uint8_t column) const;

    [[nodiscard]] uint8_t number_at(uint8_t row, uint8_t column) const;

    [[nodiscard]] uint8_t get_minimum_branching_after_setting(uint8_t row, uint8_t column, uint8_t value) const;

    [[nodiscard]] uint8_t number_of_naked_singled_created(uint8_t row, uint8_t column, uint8_t value) const;

    [[nodiscard]] uint8_t number_of_naked_singled_created(uint8_t row, uint8_t column) const;

    [[nodiscard]] NumberVector possible_if_free(uint8_t row, uint8_t column) const;

    [[nodiscard]] NumberVector find_singles_in_row(uint8_t row) const;

    [[nodiscard]] NumberVector find_singles_in_column(uint8_t column) const;

    [[nodiscard]] NumberVector find_singles_in_block(uint8_t block) const;

    std::array<RowArray, 9> m_rows{};
    std::array<std::array<NumberVector, 9>, 9> m_possible_at_position = {{{NumberVector{}.invert()}}};
    std::shared_ptr<Sudoku> m_solution{nullptr};
};

#endif // H__SUDOKU_H
