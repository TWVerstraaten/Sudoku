//
// Created by pc on 21-09-21.
//

#include "Sudoku.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>

static const uint8_t LINKED[9][9][20][2] = {
    {{{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}},
     {{0, 0}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}},
     {{0, 0}, {0, 1}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}},
     {{0, 0}, {0, 1}, {0, 2}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}},
     {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4}, {8, 4}},
     {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 6}, {0, 7}, {0, 8}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {8, 5}},
     {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 7}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 6}, {2, 7}, {2, 8}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {7, 6}, {8, 6}},
     {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 6}, {2, 7}, {2, 8}, {3, 7}, {4, 7}, {5, 7}, {6, 7}, {7, 7}, {8, 7}},
     {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {1, 6}, {1, 7}, {1, 8}, {2, 6}, {2, 7}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8}, {8, 8}}},
    {{{0, 0}, {0, 1}, {0, 2}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}},
     {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {2, 0}, {2, 1}, {2, 2}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}},
     {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {2, 0}, {2, 1}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}},
     {{0, 3}, {0, 4}, {0, 5}, {1, 0}, {1, 1}, {1, 2}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {2, 3}, {2, 4}, {2, 5}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}},
     {{0, 3}, {0, 4}, {0, 5}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {2, 3}, {2, 4}, {2, 5}, {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4}, {8, 4}},
     {{0, 3}, {0, 4}, {0, 5}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 6}, {1, 7}, {1, 8}, {2, 3}, {2, 4}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {8, 5}},
     {{0, 6}, {0, 7}, {0, 8}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 7}, {1, 8}, {2, 6}, {2, 7}, {2, 8}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {7, 6}, {8, 6}},
     {{0, 6}, {0, 7}, {0, 8}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 8}, {2, 6}, {2, 7}, {2, 8}, {3, 7}, {4, 7}, {5, 7}, {6, 7}, {7, 7}, {8, 7}},
     {{0, 6}, {0, 7}, {0, 8}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {2, 6}, {2, 7}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8}, {8, 8}}},
    {{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}},
     {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}},
     {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}},
     {{0, 3}, {0, 4}, {0, 5}, {1, 3}, {1, 4}, {1, 5}, {2, 0}, {2, 1}, {2, 2}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}},
     {{0, 3}, {0, 4}, {0, 5}, {1, 3}, {1, 4}, {1, 5}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4}, {8, 4}},
     {{0, 3}, {0, 4}, {0, 5}, {1, 3}, {1, 4}, {1, 5}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 6}, {2, 7}, {2, 8}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {8, 5}},
     {{0, 6}, {0, 7}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 7}, {2, 8}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {7, 6}, {8, 6}},
     {{0, 6}, {0, 7}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 8}, {3, 7}, {4, 7}, {5, 7}, {6, 7}, {7, 7}, {8, 7}},
     {{0, 6}, {0, 7}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8}, {8, 8}}},
    {{{0, 0}, {1, 0}, {2, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {4, 0}, {4, 1}, {4, 2}, {5, 0}, {5, 1}, {5, 2}, {6, 0}, {7, 0}, {8, 0}},
     {{0, 1}, {1, 1}, {2, 1}, {3, 0}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {4, 0}, {4, 1}, {4, 2}, {5, 0}, {5, 1}, {5, 2}, {6, 1}, {7, 1}, {8, 1}},
     {{0, 2}, {1, 2}, {2, 2}, {3, 0}, {3, 1}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {4, 0}, {4, 1}, {4, 2}, {5, 0}, {5, 1}, {5, 2}, {6, 2}, {7, 2}, {8, 2}},
     {{0, 3}, {1, 3}, {2, 3}, {3, 0}, {3, 1}, {3, 2}, {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {4, 3}, {4, 4}, {4, 5}, {5, 3}, {5, 4}, {5, 5}, {6, 3}, {7, 3}, {8, 3}},
     {{0, 4}, {1, 4}, {2, 4}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {4, 3}, {4, 4}, {4, 5}, {5, 3}, {5, 4}, {5, 5}, {6, 4}, {7, 4}, {8, 4}},
     {{0, 5}, {1, 5}, {2, 5}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 6}, {3, 7}, {3, 8}, {4, 3}, {4, 4}, {4, 5}, {5, 3}, {5, 4}, {5, 5}, {6, 5}, {7, 5}, {8, 5}},
     {{0, 6}, {1, 6}, {2, 6}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 7}, {3, 8}, {4, 6}, {4, 7}, {4, 8}, {5, 6}, {5, 7}, {5, 8}, {6, 6}, {7, 6}, {8, 6}},
     {{0, 7}, {1, 7}, {2, 7}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 8}, {4, 6}, {4, 7}, {4, 8}, {5, 6}, {5, 7}, {5, 8}, {6, 7}, {7, 7}, {8, 7}},
     {{0, 8}, {1, 8}, {2, 8}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 7}, {4, 6}, {4, 7}, {4, 8}, {5, 6}, {5, 7}, {5, 8}, {6, 8}, {7, 8}, {8, 8}}},
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {3, 1}, {3, 2}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {5, 0}, {5, 1}, {5, 2}, {6, 0}, {7, 0}, {8, 0}},
     {{0, 1}, {1, 1}, {2, 1}, {3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {5, 0}, {5, 1}, {5, 2}, {6, 1}, {7, 1}, {8, 1}},
     {{0, 2}, {1, 2}, {2, 2}, {3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 1}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {5, 0}, {5, 1}, {5, 2}, {6, 2}, {7, 2}, {8, 2}},
     {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {3, 4}, {3, 5}, {4, 0}, {4, 1}, {4, 2}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {5, 3}, {5, 4}, {5, 5}, {6, 3}, {7, 3}, {8, 3}},
     {{0, 4}, {1, 4}, {2, 4}, {3, 3}, {3, 4}, {3, 5}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {5, 3}, {5, 4}, {5, 5}, {6, 4}, {7, 4}, {8, 4}},
     {{0, 5}, {1, 5}, {2, 5}, {3, 3}, {3, 4}, {3, 5}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 6}, {4, 7}, {4, 8}, {5, 3}, {5, 4}, {5, 5}, {6, 5}, {7, 5}, {8, 5}},
     {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {3, 7}, {3, 8}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 7}, {4, 8}, {5, 6}, {5, 7}, {5, 8}, {6, 6}, {7, 6}, {8, 6}},
     {{0, 7}, {1, 7}, {2, 7}, {3, 6}, {3, 7}, {3, 8}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 8}, {5, 6}, {5, 7}, {5, 8}, {6, 7}, {7, 7}, {8, 7}},
     {{0, 8}, {1, 8}, {2, 8}, {3, 6}, {3, 7}, {3, 8}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {5, 6}, {5, 7}, {5, 8}, {6, 8}, {7, 8}, {8, 8}}},
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 1}, {4, 2}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {5, 8}, {6, 0}, {7, 0}, {8, 0}},
     {{0, 1}, {1, 1}, {2, 1}, {3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 1}, {4, 2}, {5, 0}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {5, 8}, {6, 1}, {7, 1}, {8, 1}},
     {{0, 2}, {1, 2}, {2, 2}, {3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 1}, {4, 2}, {5, 0}, {5, 1}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {5, 8}, {6, 2}, {7, 2}, {8, 2}},
     {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {3, 4}, {3, 5}, {4, 3}, {4, 4}, {4, 5}, {5, 0}, {5, 1}, {5, 2}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {5, 8}, {6, 3}, {7, 3}, {8, 3}},
     {{0, 4}, {1, 4}, {2, 4}, {3, 3}, {3, 4}, {3, 5}, {4, 3}, {4, 4}, {4, 5}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 5}, {5, 6}, {5, 7}, {5, 8}, {6, 4}, {7, 4}, {8, 4}},
     {{0, 5}, {1, 5}, {2, 5}, {3, 3}, {3, 4}, {3, 5}, {4, 3}, {4, 4}, {4, 5}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 6}, {5, 7}, {5, 8}, {6, 5}, {7, 5}, {8, 5}},
     {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {3, 7}, {3, 8}, {4, 6}, {4, 7}, {4, 8}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 7}, {5, 8}, {6, 6}, {7, 6}, {8, 6}},
     {{0, 7}, {1, 7}, {2, 7}, {3, 6}, {3, 7}, {3, 8}, {4, 6}, {4, 7}, {4, 8}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 8}, {6, 7}, {7, 7}, {8, 7}},
     {{0, 8}, {1, 8}, {2, 8}, {3, 6}, {3, 7}, {3, 8}, {4, 6}, {4, 7}, {4, 8}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {6, 8}, {7, 8}, {8, 8}}},
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7}, {6, 8}, {7, 0}, {7, 1}, {7, 2}, {8, 0}, {8, 1}, {8, 2}},
     {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 0}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7}, {6, 8}, {7, 0}, {7, 1}, {7, 2}, {8, 0}, {8, 1}, {8, 2}},
     {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 0}, {6, 1}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7}, {6, 8}, {7, 0}, {7, 1}, {7, 2}, {8, 0}, {8, 1}, {8, 2}},
     {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 0}, {6, 1}, {6, 2}, {6, 4}, {6, 5}, {6, 6}, {6, 7}, {6, 8}, {7, 3}, {7, 4}, {7, 5}, {8, 3}, {8, 4}, {8, 5}},
     {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 5}, {6, 6}, {6, 7}, {6, 8}, {7, 3}, {7, 4}, {7, 5}, {8, 3}, {8, 4}, {8, 5}},
     {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 6}, {6, 7}, {6, 8}, {7, 3}, {7, 4}, {7, 5}, {8, 3}, {8, 4}, {8, 5}},
     {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 7}, {6, 8}, {7, 6}, {7, 7}, {7, 8}, {8, 6}, {8, 7}, {8, 8}},
     {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 8}, {7, 6}, {7, 7}, {7, 8}, {8, 6}, {8, 7}, {8, 8}},
     {{0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7}, {7, 6}, {7, 7}, {7, 8}, {8, 6}, {8, 7}, {8, 8}}},
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {6, 1}, {6, 2}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {7, 8}, {8, 0}, {8, 1}, {8, 2}},
     {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {7, 8}, {8, 0}, {8, 1}, {8, 2}},
     {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 1}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {7, 8}, {8, 0}, {8, 1}, {8, 2}},
     {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {6, 4}, {6, 5}, {7, 0}, {7, 1}, {7, 2}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {7, 8}, {8, 3}, {8, 4}, {8, 5}},
     {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}, {6, 3}, {6, 4}, {6, 5}, {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 5}, {7, 6}, {7, 7}, {7, 8}, {8, 3}, {8, 4}, {8, 5}},
     {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 3}, {6, 4}, {6, 5}, {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 6}, {7, 7}, {7, 8}, {8, 3}, {8, 4}, {8, 5}},
     {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {6, 7}, {6, 8}, {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 7}, {7, 8}, {8, 6}, {8, 7}, {8, 8}},
     {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 6}, {6, 7}, {6, 8}, {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 8}, {8, 6}, {8, 7}, {8, 8}},
     {{0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 6}, {6, 7}, {6, 8}, {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {8, 6}, {8, 7}, {8, 8}}},
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 1}, {7, 2}, {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7}, {8, 8}},
     {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 1}, {7, 2}, {8, 0}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7}, {8, 8}},
     {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 1}, {7, 2}, {8, 0}, {8, 1}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7}, {8, 8}},
     {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {6, 4}, {6, 5}, {7, 3}, {7, 4}, {7, 5}, {8, 0}, {8, 1}, {8, 2}, {8, 4}, {8, 5}, {8, 6}, {8, 7}, {8, 8}},
     {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}, {6, 3}, {6, 4}, {6, 5}, {7, 3}, {7, 4}, {7, 5}, {8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 5}, {8, 6}, {8, 7}, {8, 8}},
     {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 3}, {6, 4}, {6, 5}, {7, 3}, {7, 4}, {7, 5}, {8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 6}, {8, 7}, {8, 8}},
     {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {6, 7}, {6, 8}, {7, 6}, {7, 7}, {7, 8}, {8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 7}, {8, 8}},
     {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 6}, {6, 7}, {6, 8}, {7, 6}, {7, 7}, {7, 8}, {8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 8}},
     {{0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 6}, {6, 7}, {6, 8}, {7, 6}, {7, 7}, {7, 8}, {8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7}}}};

static const uint8_t BLOCK_POSITIONS[9][9][2] = {{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}},
                                                 {{3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 1}, {4, 2}, {5, 0}, {5, 1}, {5, 2}},
                                                 {{6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 1}, {7, 2}, {8, 0}, {8, 1}, {8, 2}},
                                                 {{0, 3}, {0, 4}, {0, 5}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}},
                                                 {{3, 3}, {3, 4}, {3, 5}, {4, 3}, {4, 4}, {4, 5}, {5, 3}, {5, 4}, {5, 5}},
                                                 {{6, 3}, {6, 4}, {6, 5}, {7, 3}, {7, 4}, {7, 5}, {8, 3}, {8, 4}, {8, 5}},
                                                 {{0, 6}, {0, 7}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 6}, {2, 7}, {2, 8}},
                                                 {{3, 6}, {3, 7}, {3, 8}, {4, 6}, {4, 7}, {4, 8}, {5, 6}, {5, 7}, {5, 8}},
                                                 {{6, 6}, {6, 7}, {6, 8}, {7, 6}, {7, 7}, {7, 8}, {8, 6}, {8, 7}, {8, 8}}};

static const uint8_t TIMES_THREE_MOD_NINE[9]  = {0, 3, 6, 0, 3, 6, 0, 3, 6};
static const uint8_t DIV_THREE_TIMES_THREE[9] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
static const uint8_t INDICES_TO_BLOCK[9][9]   = {{0, 0, 0, 1, 1, 1, 2, 2, 2},
                                               {0, 0, 0, 1, 1, 1, 2, 2, 2},
                                               {0, 0, 0, 1, 1, 1, 2, 2, 2},
                                               {3, 3, 3, 4, 4, 4, 5, 5, 5},
                                               {3, 3, 3, 4, 4, 4, 5, 5, 5},
                                               {3, 3, 3, 4, 4, 4, 5, 5, 5},
                                               {6, 6, 6, 7, 7, 7, 8, 8, 8},
                                               {6, 6, 6, 7, 7, 7, 8, 8, 8},
                                               {6, 6, 6, 7, 7, 7, 8, 8, 8}};

Sudoku::Sudoku(const std::string& string) {
    assert(string.length() == 81);
    for (uint8_t row = 0; row != 9; ++row) {
        m_rows[row] = RowArray(string.substr(9 * row, 9));
    }
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            m_numbersInColumn[column].add(m_rows[row].numberAt(column));
            m_numbersInBlock[INDICES_TO_BLOCK[row][column]].add(m_rows[row].numberAt(column));
            m_numbersInRow[row].add(m_rows[row].numberAt(column));
        }
    }
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                m_possibleAtPosition[row][column] = (m_numbersInColumn[column] | m_numbersInRow[row] | m_numbersInBlock[INDICES_TO_BLOCK[row][column]]).invert();
            }
        }
    }
}

std::string Sudoku::toString() const {
    std::stringstream ss;
    for (const auto& row : m_rows) {
        for (uint8_t j = 0; j != 9; ++j) {
            uint8_t value = row.numberAt(j);
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

bool Sudoku::potentiallyRemovePossibility(uint8_t row, uint8_t column, uint8_t value) {
    if (isFree(row, column)) {
        m_possibleAtPosition[row][column].remove(value);
        if (m_possibleAtPosition[row][column].count() == 0) {
            return false;
        } else if (m_possibleAtPosition[row][column].count() == 1) {
            const auto forcedValue = m_possibleAtPosition[row][column].smallestNumber();
            if (not set(row, column, forcedValue)) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::set(uint8_t row, uint8_t column, uint8_t value) {
    assert(row < 9);
    if (m_numbersInRow[row].contains(value) || m_numbersInColumn[column].contains(value) || m_numbersInBlock[INDICES_TO_BLOCK[row][column]].contains(value)) {
        return false;
    }
    m_rows[row].set(column, value);
    m_possibleAtPosition[row][column] = NumberVector{};
    m_numbersInRow[row].add(value);
    m_numbersInColumn[column].add(value);
    m_numbersInBlock[INDICES_TO_BLOCK[row][column]].add(value);

    for (const auto& [r, c] : LINKED[row][column]) {
        m_possibleAtPosition[r][c].remove(value);
        if (isFree(r, c) && m_possibleAtPosition[r][c].count() == 0) {
            return false;
        }
    }
    for (const auto& [r, c] : LINKED[row][column]) {
        m_possibleAtPosition[r][c].remove(value);
        if (isFree(r, c) && m_possibleAtPosition[r][c].count() == 1 && not set(r, c, m_possibleAtPosition[r][c].smallestNumber())) {
            return false;
        }
    }
    return true;
}

bool Sudoku::isFree(uint8_t row, uint8_t column) const {
    assert(row < 9);
    assert(column < 9);
    return m_rows[row].positionIsFree(column);
}

bool Sudoku::fillNakedSingles(bool& singleWasFilled) {
    for (uint8_t row = 0; row != 9; ++row) {
        for (uint8_t column = 0; column != 9; ++column) {
            if (not isFree(row, column)) {
                continue;
            }
            const auto&   current = m_possibleAtPosition[row][column];
            const uint8_t count   = current.count();
            if (count == 1) {
                if (not set(row, column, current.smallestNumber())) {
                    return false;
                }
                singleWasFilled = true;
            }
        }
    }
    return true;
}

bool Sudoku::fillSingles() {
    bool wasUpdated = true;
    while (wasUpdated) {
        wasUpdated = false;
        if ((not fillNakedSingles(wasUpdated)) || (not fillAllHiddenSingles(wasUpdated))) {
            return false;
        }
    }
    return true;
}

bool Sudoku::solve() {
    if (not fillSingles()) {
        return false;
    }
    //    if (not countingCheck()) {
    //        return false;
    //    }

    uint8_t minimumPossible = std::numeric_limits<uint8_t>::max();
    uint8_t minRow          = std::numeric_limits<uint8_t>::max();
    uint8_t minColumn       = std::numeric_limits<uint8_t>::max();
    for (uint8_t row = 0; minimumPossible != 2 && row != 9; ++row) {
        for (uint8_t column = 0; minimumPossible != 2 && column != 9; ++column) {
            if (not isFree(row, column)) {
                continue;
            }
            const uint8_t count = m_possibleAtPosition[row][column].count();
            if (count < minimumPossible) {
                minimumPossible = count;
                minRow          = row;
                minColumn       = column;
            }
        }
    }
    if (minimumPossible == std::numeric_limits<uint8_t>::max()) {
        assert(isSolved());
        return true;
    }
    return solveWithSubstitutions(minRow, minColumn);
}

bool Sudoku::solveWithSubstitutions(uint8_t row, uint8_t column) {
    assert(isFree(row, column));
    const auto& allPossibilities = m_possibleAtPosition[row][column].allEntries();
    for (const auto number : allPossibilities) {
        Sudoku copy = *this;
        if (copy.set(row, column, number) && copy.solve()) {
            return true;
        }
    }
    return false;
}

Sudoku Sudoku::PRESET(size_t index) {
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

static NumberVector findSingles(const std::array<NumberVector, 9>& numbers) {
    NumberVector tooMany;
    NumberVector result;
    for (const auto& number : numbers) {
        tooMany |= (number & result);
        result |= number;
    }
    return result & (tooMany.invert());
}

NumberVector Sudoku::findSinglesInRow(uint8_t row) const {
    NumberVector tooMany;
    NumberVector result;
    const auto&  possibleInRow = m_possibleAtPosition[row];
    for (uint8_t column = 0; column != 9; ++column) {
        if (isFree(row, column)) {
            tooMany |= (possibleInRow[column] & result);
            result |= possibleInRow[column];
        }
    }
    return result & (tooMany.invert());
}

bool Sudoku::fillAllHiddenSingles(bool& singleWasFilled) {
    return fillHiddenSinglesInRows(singleWasFilled) && fillHiddenSinglesInColumns(singleWasFilled) && fillHiddenSinglesInBlocks(singleWasFilled);
}

bool Sudoku::fillHiddenSinglesInRows(bool& singleWasFilled) {
    for (uint8_t row = 0; row != 9; ++row) {
        const NumberVector singles = findSinglesInRow(row);
        for (uint8_t column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                const auto singlesInPosition = singles & m_possibleAtPosition[row][column];
                if (singlesInPosition.count() == 1) {
                    if (not set(row, column, singlesInPosition.smallestNumber())) {
                        return false;
                    } else {
                        singleWasFilled = true;
                    }
                }
            }
        }
    }
    return true;
}

bool Sudoku::fillHiddenSinglesInColumns(bool& singleWasFilled) {
    for (uint8_t column = 0; column != 9; ++column) {
        const NumberVector singles = findSinglesInColumn(column);
        for (uint8_t row = 0; row != 9; ++row) {
            if (isFree(row, column)) {
                const auto singlesInPosition = singles & m_possibleAtPosition[row][column];
                if (singlesInPosition.count() == 1) {
                    if (not set(row, column, singlesInPosition.smallestNumber())) {
                        return false;
                    } else {
                        singleWasFilled = true;
                    }
                }
            }
        }
    }
    return true;
}

NumberVector Sudoku::findSinglesInColumn(uint8_t column) const {
    return findSingles({possibleIfFree(0, column),
                        possibleIfFree(1, column),
                        possibleIfFree(2, column),
                        possibleIfFree(3, column),
                        possibleIfFree(4, column),
                        possibleIfFree(5, column),
                        possibleIfFree(6, column),
                        possibleIfFree(7, column),
                        possibleIfFree(8, column)});
}

NumberVector Sudoku::findSinglesInBlock(uint8_t firstRowOfBlock, uint8_t firstColumnOfBlock) const {
    return findSingles({possibleIfFree(firstRowOfBlock, firstColumnOfBlock),
                        possibleIfFree(firstRowOfBlock + 1, firstColumnOfBlock),
                        possibleIfFree(firstRowOfBlock + 2, firstColumnOfBlock),
                        possibleIfFree(firstRowOfBlock, firstColumnOfBlock + 1),
                        possibleIfFree(firstRowOfBlock + 1, firstColumnOfBlock + 1),
                        possibleIfFree(firstRowOfBlock + 2, firstColumnOfBlock + 1),
                        possibleIfFree(firstRowOfBlock, firstColumnOfBlock + 2),
                        possibleIfFree(firstRowOfBlock + 1, firstColumnOfBlock + 2),
                        possibleIfFree(firstRowOfBlock + 2, firstColumnOfBlock + 2)});
}

bool Sudoku::fillHiddenSinglesInBlocks(bool& singleWasFilled) {
    for (uint8_t block = 0; block != 9; ++block) {
        const NumberVector singles = findSinglesInBlock(DIV_THREE_TIMES_THREE[block], TIMES_THREE_MOD_NINE[block]);
        for (const auto& [column, row] : BLOCK_POSITIONS[block]) {
            if (isFree(row, column)) {
                const auto singlesInPosition = singles & m_possibleAtPosition[row][column];
                if (singlesInPosition.count() == 1) {
                    if (not set(row, column, singlesInPosition.smallestNumber())) {
                        return false;
                    } else {
                        singleWasFilled = true;
                    }
                }
            }
        }
    }
    return true;
}

uint8_t Sudoku::numberOfNakedSingledCreated(uint8_t row, uint8_t column, uint8_t value) const {
    uint8_t count = 0;
    for (uint8_t index = 0; index != 9; ++index) {
        count += (isFree(row, index) && m_possibleAtPosition[row][index].count() == 2 && m_possibleAtPosition[row][index].contains(value));
        count += (isFree(index, column) && m_possibleAtPosition[index][column].count() == 2 && m_possibleAtPosition[index][column].contains(value));
    }
    for (const auto& [c, r] : BLOCK_POSITIONS[INDICES_TO_BLOCK[row][column]]) {
        count += (isFree(r, c) && m_possibleAtPosition[r][c].count() == 2 && m_possibleAtPosition[r][c].contains(value));
    }
    return count - 3 * (m_possibleAtPosition[row][column].count() == 2);
}

uint8_t Sudoku::numberOfNakedSingledCreated(uint8_t row, uint8_t column) const {
    assert(isFree(row, column));
    uint8_t result = 0;
    for (const auto number : m_possibleAtPosition[row][column].allEntries()) {
        result += numberOfNakedSingledCreated(row, column, number);
    }
    return result;
}

bool Sudoku::countingCheck() const {
    for (uint8_t row = 0; row != 9; ++row) {
        NumberVector allPossibleInRow;
        uint8_t      freeInRow = 0;
        for (uint8_t column = 0; column != 9; ++column) {
            if (isFree(row, column)) {
                allPossibleInRow |= m_possibleAtPosition[row][column];
                ++freeInRow;
                if (allPossibleInRow.count() < freeInRow) {
                    return false;
                }
            }
        }
    }
    for (uint8_t column = 0; column != 9; ++column) {
        NumberVector allPossibleInColumn;
        uint8_t      freeInColumn = 0;
        for (uint8_t row = 0; row != 9; ++row) {
            if (isFree(row, column)) {
                allPossibleInColumn |= m_possibleAtPosition[row][column];
                ++freeInColumn;
                if (allPossibleInColumn.count() < freeInColumn) {
                    return false;
                }
            }
        }
    }
    for (const auto& block : BLOCK_POSITIONS) {
        NumberVector allPossibleInBlock;
        uint8_t      freeInBlock = 0;
        for (const auto& [row, column] : block) {
            if (isFree(row, column)) {
                allPossibleInBlock |= m_possibleAtPosition[row][column];
                ++freeInBlock;
                if (allPossibleInBlock.count() < freeInBlock) {
                    return false;
                }
            }
        }
    }
    return true;
}

const NumberVector& Sudoku::possibleIfFree(uint8_t row, uint8_t column) const {
    return m_possibleAtPosition[row][column];
}

bool Sudoku::findPointingSets(bool& wasUpdated) {
    for (uint8_t row = 0; row < 9; row += 3) {
        for (uint8_t column = 0; column < 9; column += 3) {
            const NumberVector possibleInFirstRow  = possibleIfFree(row, column) | possibleIfFree(row, column + 1) | possibleIfFree(row, column + 2);
            const NumberVector possibleInSecondRow = possibleIfFree(row + 1, column) | possibleIfFree(row + 1, column + 1) | possibleIfFree(row + 1, column + 2);
            const NumberVector possibleInThirdRow  = possibleIfFree(row + 2, column) | possibleIfFree(row + 2, column + 1) | possibleIfFree(row + 2, column + 2);

            const NumberVector uniqueInRow[3]{possibleInFirstRow & (possibleInSecondRow.invert()) & (possibleInThirdRow.invert()),
                                              possibleInSecondRow & (possibleInFirstRow.invert()) & (possibleInThirdRow.invert()),
                                              possibleInThirdRow & (possibleInSecondRow.invert()) & (possibleInFirstRow.invert())};
            for (uint8_t rowOffset = 0; rowOffset != 3; ++rowOffset) {
                for (const auto value : uniqueInRow[rowOffset].allEntries()) {
                    for (uint8_t columnToRemove = 0; columnToRemove != 9; ++columnToRemove) {
                        if (columnToRemove < column || columnToRemove > column + 2) {
                            if (not removeIfPresent(row + rowOffset, columnToRemove, value, wasUpdated)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    for (uint8_t row = 0; row != 9; ++row) {
        const NumberVector possibleInFirstBlock  = possibleIfFree(row, 0) | possibleIfFree(row, 1) | possibleIfFree(row, 2);
        const NumberVector possibleInSecondBlock = possibleIfFree(row, 3) | possibleIfFree(row, 4) | possibleIfFree(row, 5);
        const NumberVector possibleInThirdBlock  = possibleIfFree(row, 6) | possibleIfFree(row, 7) | possibleIfFree(row, 8);
        const NumberVector uniquePerBlock[3]     = {possibleInFirstBlock & (possibleInSecondBlock.invert()) & (possibleInThirdBlock.invert()),
                                                possibleInSecondBlock & (possibleInFirstBlock.invert()) & (possibleInThirdBlock.invert()),
                                                possibleInThirdBlock & (possibleInSecondBlock.invert()) & (possibleInFirstBlock.invert())};
        for (uint8_t columnBlock = 0; columnBlock != 3; ++columnBlock) {
            for (const auto value : uniquePerBlock[columnBlock].allEntries()) {
                for (uint8_t rowToRemove = DIV_THREE_TIMES_THREE[row]; rowToRemove != DIV_THREE_TIMES_THREE[row] + 3; ++rowToRemove) {
                    if (rowToRemove != row) {
                        for (uint8_t column = 3 * columnBlock; column != 3 * columnBlock + 3; ++column) {
                            if (not removeIfPresent(rowToRemove, column, value, wasUpdated)) {
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
            const NumberVector possibleInFirstColumn  = possibleIfFree(row, column) | possibleIfFree(row + 1, column) | possibleIfFree(row + 2, column);
            const NumberVector possibleInSecondColumn = possibleIfFree(row, column + 1) | possibleIfFree(row + 1, column + 1) | possibleIfFree(row + 2, column + 1);
            const NumberVector possibleInThirdColumn  = possibleIfFree(row, column + 2) | possibleIfFree(row + 1, column + 2) | possibleIfFree(row + 2, column + 2);
            const NumberVector onlyInFirstColumn[3]   = {possibleInFirstColumn & (possibleInSecondColumn.invert()) & (possibleInThirdColumn.invert()),
                                                       possibleInSecondColumn & (possibleInFirstColumn.invert()) & (possibleInThirdColumn.invert()),
                                                       possibleInThirdColumn & (possibleInSecondColumn.invert()) & (possibleInFirstColumn.invert())};
            for (uint8_t k = 0; k != 3; ++k) {
                for (uint8_t rowToRemove = 0; rowToRemove != 9; ++rowToRemove) {
                    if (rowToRemove < row || rowToRemove > row + 2) {
                        for (const auto inFirst : onlyInFirstColumn[k].allEntries()) {
                            if (not removeIfPresent(rowToRemove, column + k, inFirst, wasUpdated)) {
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

bool Sudoku::removeIfPresent(uint8_t row, uint8_t column, uint8_t value, bool& wasRemoved) {
    if (isFree(row, column)) {
        if (m_possibleAtPosition[row][column].contains(value)) {
            wasRemoved = true;
        }
        m_possibleAtPosition[row][column].remove(value);
        if (m_possibleAtPosition[row][column].count() == 0) {
            return false;
        } else if (m_possibleAtPosition[row][column].count() == 1) {
            const auto forcedValue = m_possibleAtPosition[row][column].smallestNumber();
            if (not set(row, column, forcedValue)) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::isSolved() const {
    for (uint8_t row = 0; row != 9; ++row) {
        std::set<uint8_t> allInRow;
        for (uint8_t column = 0; column != 9; ++column) {
            allInRow.insert(m_rows[row].numberAt(column));
        }
        if (allInRow.size() != 9 || *std::max_element(allInRow.begin(), allInRow.end()) > 9 || *std::min_element(allInRow.begin(), allInRow.end()) == 0) {
            std::cout << "Row error in row " << static_cast<int>(row) << "\n";
            return false;
        }
    }
    for (uint8_t column = 0; column != 9; ++column) {
        std::set<uint8_t> allInColumn;
        for (uint8_t row = 0; row != 9; ++row) {
            allInColumn.insert(m_rows[row].numberAt(column));
        }
        if (allInColumn.size() != 9 || *std::max_element(allInColumn.begin(), allInColumn.end()) > 9 || *std::min_element(allInColumn.begin(), allInColumn.end()) == 0) {
            std::cout << "Column error in column " << static_cast<int>(column) << "\n";
            return false;
        }
    }
    for (uint8_t block = 0; block != 9; ++block) {
        std::set<uint8_t> allInColumn;
        for (const auto [row, column] : BLOCK_POSITIONS[block]) {
            allInColumn.insert(m_rows[row].numberAt(column));
        }
        if (allInColumn.size() != 9 || *std::max_element(allInColumn.begin(), allInColumn.end()) > 9 || *std::min_element(allInColumn.begin(), allInColumn.end()) == 0) {
            std::cout << "Block error in block " << static_cast<int>(block) << "\n";
            return false;
        }
    }
    return true;
}
