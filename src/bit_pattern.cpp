//
// Created by konstantin on 31.05.23.
//

#include <initializer_list>
#include <iostream>
#include "bit_pattern.hpp"

pine::bit_pattern::bit_pattern(std::initializer_list<bool> list) {
    m_Len = (list.size() / 8) + 1;
    m_Bits.resize(m_Len, 0);
    int counter = 0;
    for (const auto& value : list) {
        if (value)
            m_Bits[counter / 8] |= (1 << (counter % 8));
        counter++;
    }
}

pine::bit_pattern::bit_pattern(std::initializer_list<int> list) {

}

std::ostream& pine::operator<<(std::ostream& os, const pine::bit_pattern& pattern) {
    for (int i = 0; i < pattern.m_Len; i++) {
        for (int j = 0; j < 8; j++) {
            uint16_t bit = (pattern.m_Bits[i] & (1 << j)) > 0 ? 1 : 0;
            os << bit;
        }
    }
    return os;
}