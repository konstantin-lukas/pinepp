//
// Created by konstantin on 31.05.23.
//

#include <iostream>
#include "../inc/base64.hpp"

std::string pinepp::base64_encode(const std::string& input) {
    std::string output;
    return output;
}
std::string pinepp::base64_encode(const pinepp::bit_pattern& input, bool padding) {
    auto fill_zeroes = (6 - (input.size() % 6)) % 6;
    std::string base64;
    size_t i;
    for (i = 0; i < input.size() / 6; ++i) {
        uint8_t index = 0;
        for (size_t j = 0; j < 6; ++j) {
            index |= input[6 * i + j] << (5 - j);
        }
        base64.push_back(BASE64_TABLE[index]);
    }
    uint8_t index = 0;
    for (size_t k = 0; k < 6 - fill_zeroes; ++k) {
        index |= input[6 * i + k] << (5 - k);
    }
    base64.push_back(BASE64_TABLE[index]);
    if (padding)
       while (base64.length() % 4 != 0) {
           base64.push_back('=');
       }
    return base64;
}
