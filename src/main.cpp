//
// Created by konstantin on 31.05.23.
//
#include <iostream>
#include "base64.hpp"
#include "bit_pattern.hpp"

int main() {
    pine::bit_pattern pattern{false, true, false, false, false, false, false, true, true, true, true, false};
    for(int x : pattern) {
        std::cout << x;
    }
    std::cout << '\n' << pine::base64_encode(pattern) << '\n';
}