//
// Created by pc on 21-09-21.
//

#include "Sudoku.h"

#include "SudokuStaticArrays.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <set>
#include <sstream>

Sudoku::Sudoku(const std::string& string) {
    m_solution.reset(new Sudoku(*this));
    assert(string.length() == 81);
    for (uint8_t row = 0; row != 9; ++row) {
        m_rows[row] = RowArray(string.substr(9 * row, 9));
    }
    std::array<NumberVector, 9> numbers_in_row;
    std::array<NumberVector, 9> numbers_in_column;
    std::array<NumberVector, 9> numbers_in_block;
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            numbers_in_column[column].add(number_at(row, column));
            numbers_in_block[g_indices_to_block[row][column]].add(number_at(row, column));
            numbers_in_row[row].add(number_at(row, column));
        }
    }
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (is_free(row, column)) {
                m_possible_at_position[row][column] = (numbers_in_column[column] | numbers_in_row[row] | numbers_in_block[g_indices_to_block[row][column]]).invert();
            }
        }
    }
}

std::string Sudoku::to_string() const {
    std::stringstream ss;
    for (const auto& row : m_rows) {
        for (uint8_t j = 0; j != 9; ++j) {
            uint8_t value = row.number_at(j);
            if (value == 0) {
                ss << ".";
            } else {
                ss << static_cast<unsigned short>(value);
            }
        }
        ss << "\n";
    }
    return ss.str();
}

bool Sudoku::set(uint8_t row, uint8_t column, uint8_t value) {
    assert(row < 9);
    m_rows[row].set(column, value);
    for (const auto& [r, c] : g_linked[row][column]) {
        m_possible_at_position[r][c].remove(value);
        if (not has_possibilities(row, column)) {
            return false;
        }
    }
    return std::all_of(std::begin(g_linked[row][column]), std::end(g_linked[row][column]), [this](const auto(&row_column_pair)[2]) {
        const auto [r, c] = row_column_pair;
        return (not(is_free(r, c) && m_possible_at_position[r][c].count() == 1 && not set(r, c, m_possible_at_position[r][c].smallest_number())));
    });
}

bool Sudoku::fill_naked_singles(bool& single_was_filled) {
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (is_free(row, column)) {
                const auto&   current = m_possible_at_position[row][column];
                const uint8_t count   = current.count();
                if (count == 1) {
                    if (not set(row, column, current.smallest_number())) {
                        return false;
                    }
                    single_was_filled = true;
                }
            }
        }
    }
    return true;
}

bool Sudoku::deterministic_inferences() {
    bool was_updated     = false;
    bool any_was_updated = false;
    if ((not fill_naked_singles(was_updated))) {
        return false;
    }
    any_was_updated |= was_updated;
    if (not fill_all_hidden_singles(was_updated)) {
        return false;
    }
    any_was_updated |= was_updated;
    if ((not any_was_updated) && (not find_pointing_sets(any_was_updated))) {
        return false;
    }
    if (any_was_updated) {
        return deterministic_inferences();
    }
    return counting_check();
}

bool Sudoku::solve() {
    if (not deterministic_inferences()) {
        return false;
    }

    uint8_t minimum_possible    = std::numeric_limits<uint8_t>::max();
    uint8_t min_row             = std::numeric_limits<uint8_t>::max();
    uint8_t min_column          = std::numeric_limits<uint8_t>::max();
    float   min_reduction_score = std::numeric_limits<float>::max();
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (is_free(row, column)) {
                const uint8_t count = m_possible_at_position[row][column].count();
                if (count <= minimum_possible) {
                    const float reduction_score = get_reduction_score(row, column);
                    if (count < minimum_possible || reduction_score < min_reduction_score) {
                        minimum_possible    = count;
                        min_row             = row;
                        min_column          = column;
                        min_reduction_score = reduction_score;
                    }
                }
            }
        }
    }
    if (minimum_possible == std::numeric_limits<uint8_t>::max()) {
        assert(is_solved());
        *m_solution = *this;
        return true;
    }
    return try_by_substituting(min_row, min_column);
}

bool Sudoku::try_by_substituting(uint8_t row, uint8_t column) {
    assert(is_free(row, column));
    std::vector<Sudoku> tries;
    tries.reserve(m_possible_at_position[row][column].count());
    for (const auto number : m_possible_at_position[row][column].all_entries()) {
        Sudoku copy = *this;
        if (copy.set(row, column, number)) {
            tries.emplace_back(std::move(copy));
        }
    }
    for (auto& copy : tries) {
        if (copy.solve()) {
            return true;
        }
    }
    return false;
}

Sudoku Sudoku::preset(size_t index) {
    switch (index) {
        case 0:
            return Sudoku{"123456789"
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."
                          "........."};
        case 1:
            return Sudoku{"53..7...."
                          "6..195..."
                          ".98....6."
                          "8...6...3"
                          "4..8.3..1"
                          "7...2...6"
                          ".6....28."
                          "...419..5"
                          "....8..79"};
        case 2:
            return Sudoku{"...4.8..."
                          ".6..7..1."
                          "7.2.9.5.4"
                          ".9.7.4.3."
                          "..7.5.8.."
                          ".8.9.6.5."
                          "9.4.1.7.8"
                          ".7..6..4."
                          "...2.7..."};
        case 3:
            return Sudoku{"53..7...."
                          "6........"
                          ".98....6."
                          "8.......3"
                          "4.......1"
                          "7.......6"
                          ".6.....8."
                          "........5"
                          "....8...."};
        case 4:
            return Sudoku{".....8..."
                          ".6..7..1."
                          "7.2.9.5.4"
                          ".9.7.4.3."
                          "..7.5...."
                          "...9.6.5."
                          "..4.1...8"
                          ".7..6...."
                          "...2.7..."};
        default:
            exit(1);
    }
}

NumberVector Sudoku::find_singles_in_block(uint8_t block) const {
    NumberVector too_many;
    NumberVector result;
    for (const auto [column, row] : g_block_positions[block]) {
        if (is_free(row, column)) {
            const auto& possible = m_possible_at_position[row][column];
            too_many |= (possible & result);
            result |= possible;
        }
    }
    return result & (too_many.invert());
}

NumberVector Sudoku::find_singles_in_column(uint8_t column) const {
    NumberVector too_many;
    NumberVector result;
    for (uint8_t row = 0; row != 9; ++row) {
        if (is_free(row, column)) {
            const auto& possible = m_possible_at_position[row][column];
            too_many |= (possible & result);
            result |= possible;
        }
    }
    return result & (too_many.invert());
}

NumberVector Sudoku::find_singles_in_row(uint8_t row) const {
    NumberVector too_many;
    NumberVector result;
    const auto&  possible_in_row = m_possible_at_position[row];
    for (uint8_t column = 0; column != 9; ++column) {
        if (is_free(row, column)) {
            too_many |= (possible_in_row[column] & result);
            result |= possible_in_row[column];
        }
    }
    return result & (too_many.invert());
}

bool Sudoku::fill_all_hidden_singles(bool& single_was_filled) {
    return fill_hidden_singles_in_rows(single_was_filled) && fill_hidden_singles_in_columns(single_was_filled) && fill_hidden_singles_in_blocks(single_was_filled);
}

bool Sudoku::fill_hidden_singles_in_rows(bool& single_was_filled) {
    for (uint8_t row = 0; row != 9; ++row) {
        const NumberVector singles = find_singles_in_row(row);
        for (uint8_t column = 0; column != 9; ++column) {
            if (is_free(row, column)) {
                const auto singles_in_position = singles & m_possible_at_position[row][column];
                if (singles_in_position.count() == 1) {
                    if (not set(row, column, singles_in_position.smallest_number())) {
                        return false;
                    } else {
                        single_was_filled = true;
                    }
                }
            }
        }
    }
    return true;
}

bool Sudoku::fill_hidden_singles_in_columns(bool& single_was_filled) {
    for (uint8_t column = 0; column != 9; ++column) {
        const NumberVector singles = find_singles_in_column(column);
        for (uint8_t row = 0; row != 9; ++row) {
            if (is_free(row, column)) {
                const auto singles_in_position = singles & m_possible_at_position[row][column];
                if (singles_in_position.count() == 1) {
                    if (not set(row, column, singles_in_position.smallest_number())) {
                        return false;
                    } else {
                        single_was_filled = true;
                    }
                }
            }
        }
    }
    return true;
}

bool Sudoku::fill_hidden_singles_in_blocks(bool& single_was_filled) {
    for (uint8_t block = 0; block != 9; ++block) {
        const NumberVector singles = find_singles_in_block(block);
        for (const auto& [column, row] : g_block_positions[block]) {
            if (is_free(row, column)) {
                const auto singles_in_position = singles & m_possible_at_position[row][column];
                if (singles_in_position.count() == 1) {
                    if (not set(row, column, singles_in_position.smallest_number())) {
                        return false;
                    } else {
                        single_was_filled = true;
                    }
                }
            }
        }
    }
    return true;
}

uint8_t Sudoku::number_of_naked_singled_created(uint8_t row, uint8_t column, uint8_t value) const {
    uint8_t count = 0;
    for (uint8_t index = 0; index != 9; ++index) {
        count += (is_free(row, index) && m_possible_at_position[row][index].count() == 2 && m_possible_at_position[row][index].contains(value));
        count += (is_free(index, column) && m_possible_at_position[index][column].count() == 2 && m_possible_at_position[index][column].contains(value));
    }
    for (const auto& [c, r] : g_block_positions[g_indices_to_block[row][column]]) {
        count += (is_free(r, c) && m_possible_at_position[r][c].count() == 2 && m_possible_at_position[r][c].contains(value));
    }
    return count - 3 * (m_possible_at_position[row][column].count() == 2);
}

uint8_t Sudoku::number_of_naked_singled_created(uint8_t row, uint8_t column) const {
    assert(is_free(row, column));
    uint8_t result = 0;
    for (const auto number : m_possible_at_position[row][column].all_entries()) {
        result += number_of_naked_singled_created(row, column, number);
    }
    return result;
}

bool Sudoku::row_counting_check(uint8_t row) const {
    NumberVector all_possible_in_row;
    uint8_t      free_in_row = 0;
    for (uint8_t column = 0; column != 9; ++column) {
        if (is_free(row, column)) {
            all_possible_in_row |= m_possible_at_position[row][column];
            ++free_in_row;
            if (all_possible_in_row.count() < free_in_row) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::column_counting_check(uint8_t column) const {
    NumberVector all_possible_in_column;
    uint8_t      free_in_column = 0;
    for (uint8_t row = 0; row != 9; ++row) {
        if (is_free(row, column)) {
            all_possible_in_column |= m_possible_at_position[row][column];
            ++free_in_column;
            if (all_possible_in_column.count() < free_in_column) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::block_counting_check(uint8_t block) const {
    NumberVector all_possible_in_block;
    uint8_t      free_in_block = 0;
    for (const auto& [row, column] : g_block_positions[block]) {
        if (is_free(row, column)) {
            all_possible_in_block |= m_possible_at_position[row][column];
            ++free_in_block;
            if (all_possible_in_block.count() < free_in_block) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::counting_check() const {
    for (uint8_t index = 0; index != 9; ++index) {
        if ((not row_counting_check(index)) || (not column_counting_check(index)) || (not block_counting_check(index))) {
            return false;
        }
    }
    return true;
}

NumberVector Sudoku::possible_if_free(uint8_t row, uint8_t column) const {
    if (is_free(row, column)) {
        return m_possible_at_position[row][column];
    }
    return NumberVector{};
}

bool Sudoku::find_pointing_sets(bool& was_updated) {
    for (uint8_t row = 0; row < 9; row += 3) {
        for (uint8_t column = 0; column < 9; column += 3) {
            const NumberVector possible_in_first_row  = possible_if_free(row, column) | possible_if_free(row, column + 1) | possible_if_free(row, column + 2);
            const NumberVector possible_in_second_row = possible_if_free(row + 1, column) | possible_if_free(row + 1, column + 1) | possible_if_free(row + 1, column + 2);
            const NumberVector possible_in_third_row  = possible_if_free(row + 2, column) | possible_if_free(row + 2, column + 1) | possible_if_free(row + 2, column + 2);

            const NumberVector unique_in_row[3]{possible_in_first_row & (possible_in_second_row.invert()) & (possible_in_third_row.invert()),
                                                possible_in_second_row & (possible_in_first_row.invert()) & (possible_in_third_row.invert()),
                                                possible_in_third_row & (possible_in_second_row.invert()) & (possible_in_first_row.invert())};
            for (uint8_t row_offset = 0; row_offset != 3; ++row_offset) {
                for (const auto value : unique_in_row[row_offset].all_entries()) {
                    for (uint8_t column_to_remove = 0; column_to_remove != 9; ++column_to_remove) {
                        if (column_to_remove < column || column_to_remove > column + 2) {
                            if (not remove_if_present(row + row_offset, column_to_remove, value, was_updated)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    for (uint8_t row = 0; row != 9; ++row) {
        const NumberVector possible_in_first_block  = possible_if_free(row, 0) | possible_if_free(row, 1) | possible_if_free(row, 2);
        const NumberVector possible_in_second_block = possible_if_free(row, 3) | possible_if_free(row, 4) | possible_if_free(row, 5);
        const NumberVector possible_in_third_block  = possible_if_free(row, 6) | possible_if_free(row, 7) | possible_if_free(row, 8);
        const NumberVector unique_per_block[3]      = {possible_in_first_block & (possible_in_second_block.invert()) & (possible_in_third_block.invert()),
                                                  possible_in_second_block & (possible_in_first_block.invert()) & (possible_in_third_block.invert()),
                                                  possible_in_third_block & (possible_in_second_block.invert()) & (possible_in_first_block.invert())};
        for (uint8_t column_block = 0; column_block != 3; ++column_block) {
            for (const auto value : unique_per_block[column_block].all_entries()) {
                for (uint8_t row_to_remove = g_div_three_times_three[row]; row_to_remove != g_div_three_times_three[row] + 3; ++row_to_remove) {
                    if (row_to_remove != row) {
                        for (uint8_t column = 3 * column_block; column != 3 * column_block + 3; ++column) {
                            if (not remove_if_present(row_to_remove, column, value, was_updated)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    for (uint8_t column = 0; column != 9; column += 3) {
        for (uint8_t row = 0; row != 9; row += 3) {
            const NumberVector possible_in_first_column  = possible_if_free(row, column) | possible_if_free(row + 1, column) | possible_if_free(row + 2, column);
            const NumberVector possible_in_second_column = possible_if_free(row, column + 1) | possible_if_free(row + 1, column + 1) | possible_if_free(row + 2, column + 1);
            const NumberVector possible_in_third_column  = possible_if_free(row, column + 2) | possible_if_free(row + 1, column + 2) | possible_if_free(row + 2, column + 2);
            const NumberVector only_in_first_column[3]   = {possible_in_first_column & (possible_in_second_column.invert()) & (possible_in_third_column.invert()),
                                                          possible_in_second_column & (possible_in_first_column.invert()) & (possible_in_third_column.invert()),
                                                          possible_in_third_column & (possible_in_second_column.invert()) & (possible_in_first_column.invert())};
            for (uint8_t k = 0; k != 3; ++k) {
                for (uint8_t row_to_remove = 0; row_to_remove != 9; ++row_to_remove) {
                    if (row_to_remove < row || row_to_remove > row + 2) {
                        for (const auto in_first : only_in_first_column[k].all_entries()) {
                            if (not remove_if_present(row_to_remove, column + k, in_first, was_updated)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    for (uint8_t column = 0; column != 9; ++column) {
        const NumberVector possible_in_first_block  = possible_if_free(0, column) | possible_if_free(1, column) | possible_if_free(2, column);
        const NumberVector possible_in_second_block = possible_if_free(3, column) | possible_if_free(4, column) | possible_if_free(5, column);
        const NumberVector possible_in_third_block  = possible_if_free(6, column) | possible_if_free(7, column) | possible_if_free(8, column);
        const NumberVector unique_per_block[3]      = {possible_in_first_block & (possible_in_second_block.invert()) & (possible_in_third_block.invert()),
                                                  possible_in_second_block & (possible_in_first_block.invert()) & (possible_in_third_block.invert()),
                                                  possible_in_third_block & (possible_in_second_block.invert()) & (possible_in_first_block.invert())};
        for (uint8_t row_block = 0; row_block != 3; ++row_block) {
            for (const auto value : unique_per_block[row_block].all_entries()) {
                for (uint8_t column_to_remove = g_div_three_times_three[column]; column_to_remove != g_div_three_times_three[column] + 3; ++column_to_remove) {
                    if (column_to_remove != column) {
                        for (uint8_t row = 3 * row_block; row != 3 * row_block + 3; ++row) {
                            if (not remove_if_present(row, column_to_remove, value, was_updated)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool Sudoku::remove_if_present(uint8_t row, uint8_t column, uint8_t value, bool& was_removed) {
    if (is_free(row, column)) {
        if (m_possible_at_position[row][column].contains(value)) {
            was_removed = true;
        }
        m_possible_at_position[row][column].remove(value);
        if (m_possible_at_position[row][column].count() == 0) {
            return false;
        } else if (m_possible_at_position[row][column].count() == 1) {
            const auto forced_value = m_possible_at_position[row][column].smallest_number();
            if (not set(row, column, forced_value)) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::is_solved() const {
    for (uint8_t row = 0; row != 9; ++row) {
        std::set<uint8_t> all_in_row;
        for (uint8_t column = 0; column != 9; ++column) {
            all_in_row.insert(number_at(row, column));
        }
        if (all_in_row.size() != 9 || *std::max_element(all_in_row.begin(), all_in_row.end()) > 9 || *std::min_element(all_in_row.begin(), all_in_row.end()) == 0) {
            return false;
        }
    }
    for (uint8_t column = 0; column != 9; ++column) {
        std::set<uint8_t> all_in_column;
        for (uint8_t row = 0; row != 9; ++row) {
            all_in_column.insert(number_at(row, column));
        }
        if (all_in_column.size() != 9 || *std::max_element(all_in_column.begin(), all_in_column.end()) > 9 || *std::min_element(all_in_column.begin(), all_in_column.end()) == 0) {
            return false;
        }
    }
    for (uint8_t block = 0; block != 9; ++block) {
        std::set<uint8_t> all_in_column;
        for (const auto [row, column] : g_block_positions[block]) {
            all_in_column.insert(number_at(row, column));
        }
        if (all_in_column.size() != 9 || *std::max_element(all_in_column.begin(), all_in_column.end()) > 9 || *std::min_element(all_in_column.begin(), all_in_column.end()) == 0) {
            return false;
        }
    }
    return true;
}

bool Sudoku::is_consistent() const {
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (is_free(row, column)) {
                for (const auto& [r, c] : g_linked[row][column]) {
                    if (not is_free(r, c) && m_possible_at_position[row][column].contains(m_rows[r].number_at(c))) {
                        return false;
                    }
                }
            } else {
                for (const auto& [r, c] : g_linked[row][column]) {
                    if (not is_free(r, c) && m_rows[r].number_at(c) == number_at(row, column)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

float Sudoku::get_reduction_score(uint8_t row, uint8_t column) const {
    assert(is_free(row, column));
    size_t            result = 0;
    static const auto square = [](const size_t a) { return a * a; };
    for (const auto value : m_possible_at_position[row][column].all_entries()) {
        result += square(get_minimum_branching_after_setting(row, column, value));
    }
    return static_cast<float>(result) / static_cast<float>(m_possible_at_position[row][column].count());
}

uint8_t Sudoku::get_minimum_branching_after_setting(uint8_t row, uint8_t column, uint8_t value) const {
    size_t minimum_branch = std::numeric_limits<size_t>::max();
    for (const auto& [r, c] : g_linked[row][column]) {
        if (is_free(r, c)) {
            minimum_branch = std::min(minimum_branch, static_cast<size_t>(m_possible_at_position[r][c].count() - m_possible_at_position[r][c].contains(value)));
        }
    }
    return minimum_branch;
}

bool Sudoku::has_possibilities(uint8_t row, uint8_t column) {
    return not m_possible_at_position[row][column].is_empty();
}

std::shared_ptr<Sudoku> Sudoku::solution() const {
    return m_solution;
}
