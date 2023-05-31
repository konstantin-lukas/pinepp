//
// Created by konstantin on 31.05.23.
//
#include <iostream>
#include "bit_pattern.hpp"

int main() {
    pine::bit_pattern pattern{false, true, false, false, false, false, false, true, true, true, true};
    std::cout << pattern << '\n';
}