//
// Created by konstantin on 31.05.23.
//

#include <initializer_list>
#include <iostream>
#include "bit_pattern.hpp"

pine_pp::bit_pattern::bit_pattern(std::initializer_list<bool> list) {
    m_Len = list.size();
    m_RawBytes.resize((list.size() / 8) + 1, 0);
    int counter = 0;
    for (const auto& value : list) {
        if (value)
            m_RawBytes[counter / 8] |= (1 << (counter % 8));
        counter++;
    }
}

pine_pp::bit_pattern::iterator::iterator(size_t index, bit_pattern* ptr)
    : mp_BitPattern(ptr), m_Index(index) {}

pine_pp::bit_pattern::iterator pine_pp::bit_pattern::begin() {
    return iterator{0, this};
}

pine_pp::bit_pattern::iterator pine_pp::bit_pattern::end() {
    return iterator{m_Len, this};
}


pine_pp::bit_pattern::iterator& pine_pp::bit_pattern::iterator::operator++() {
    m_Index++;
    return *this;
}

// TODO: POST FIX INCREMENT
// TODO: INDEX OP

bool pine_pp::bit_pattern::iterator::operator==(pine_pp::bit_pattern::iterator other) const {
    return this->m_Index == other.m_Index;
}
bool pine_pp::bit_pattern::iterator::operator!=(pine_pp::bit_pattern::iterator other) const {
    return this->m_Index != other.m_Index;
}

int pine_pp::bit_pattern::iterator::operator*() const {
    return mp_BitPattern->m_RawBytes[m_Index / 8] & (1 << (m_Index % 8)) ? 1 : 0;
}

pine_pp::bit_pattern::bit_pattern(std::initializer_list<int> list) {

}

size_t pine_pp::bit_pattern::size() const {
    return m_Len;
}


int pine_pp::bit_pattern::operator[](int index) const {
    return m_RawBytes[index / 8] & (1 << (index % 8)) ? 1 : 0;
}

namespace pine_pp {
    std::ostream &operator<<(std::ostream &os, const bit_pattern &pattern) {
        for (size_t i = 0; i < pattern.m_Len; i++)
            os << ((pattern.m_RawBytes[i / 8] & (1 << (i % 8))) > 0 ? 1 : 0);
        return os;
    }
}
