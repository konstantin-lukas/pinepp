//
// Created by konstantin on 31.05.23.
//

#include <iostream>
#include <ranges>
#include <regex>
#include "bit_pattern.hpp"


pinepp::bit_pattern::bit_pattern() : m_Len(0) {}

pinepp::bit_pattern::bit_pattern(const std::string& str) {
    if (std::regex pattern{"^[01]+$"}; std::regex_match(str, pattern)) {
        m_Len = str.size();
        m_RawBytes.resize((m_Len / 8) + 1, 0);
        int counter = 0;
        for (auto c : std::ranges::reverse_view(str)) {
            if (c == '1')
                m_RawBytes[counter / 8] |= (1 << (counter % 8));
            counter++;
        }
    } else if (pattern = "^0[xX][0-9a-fA-F]+$"; std::regex_match(str, pattern)) {
        m_Len = (str.size() - 2) * 4;
        m_RawBytes.resize((m_Len / 8) + 1, 0);
        size_t counter = 0;
        auto it = str.rbegin();
        while (counter < m_Len) {
            uint8_t digit = 0;

            if (*it >= '1' && *it <= '9')
                digit = *it - '1' + 1;
            else if (*it >= 'a' && *it <= 'f')
                digit = *it - 'a' + 10;
            else if (*it >= 'A' && *it <= 'F')
                digit = *it - 'A' + 10;

            if (counter % 2 == 0)
                m_RawBytes[counter / 2] |= digit;
            else
                m_RawBytes[counter / 2] |= digit << 4;

            counter++;
            ++it;
        }
    }
}

pinepp::bit_pattern::bit_pattern(bit_pattern&& other) noexcept {
    m_RawBytes = std::move(other.m_RawBytes);
    m_Len = other.m_Len;
}

size_t pinepp::bit_pattern::size() const {
    return m_Len;
}


void pinepp::bit_pattern::set_bit(int index, bool value) {
    if (value)
        m_RawBytes[index / 8] |= (1 << (index % 8));
    else
        m_RawBytes[index / 8] &= ~(1 << (index % 8));
}

void pinepp::bit_pattern::reverse() {
    std::stringstream o{};
    o << *this;
    auto c = o.str();
    std::reverse(c.begin(), c.end());
    *this = bit_pattern{c};
}

pinepp::bit_pattern& pinepp::bit_pattern::operator=(pinepp::bit_pattern&& other) noexcept {
    if (&other == this)
        return *this;
    this->m_Len = other.m_Len;
    this->m_RawBytes = std::move(other.m_RawBytes);
    other.m_Len = 0;
    return *this;
}

int pinepp::bit_pattern::operator[](int index) const {
    return m_RawBytes[index / 8] & (1 << (index % 8)) ? 1 : 0;
}

pinepp::bit_pattern pinepp::bit_pattern::operator&(const bit_pattern& other) const {
    const auto& shorterPattern = this->m_Len < other.m_Len ? *this : other;
    const auto& longerPattern = this->m_Len < other.m_Len ? other : *this;
    bit_pattern rv{shorterPattern};

    for (decltype(shorterPattern.m_RawBytes)::size_type i = 0; i < shorterPattern.m_RawBytes.size(); ++i)
        rv.m_RawBytes[i] = shorterPattern.m_RawBytes[i] & longerPattern.m_RawBytes[i];

    return rv;
}

pinepp::bit_pattern pinepp::bit_pattern::operator|(const bit_pattern& other) const {
    const auto& shorterPattern = this->m_Len < other.m_Len ? *this : other;
    const auto& longerPattern = this->m_Len < other.m_Len ? other : *this;
    bit_pattern rv{shorterPattern};

    for (decltype(shorterPattern.m_RawBytes)::size_type i = 0; i < shorterPattern.m_RawBytes.size(); ++i)
        rv.m_RawBytes[i] = shorterPattern.m_RawBytes[i] | longerPattern.m_RawBytes[i];

    return rv;
}

pinepp::bit_pattern pinepp::bit_pattern::operator^(const bit_pattern& other) const {
    const auto& shorterPattern = this->m_Len < other.m_Len ? *this : other;
    const auto& longerPattern = this->m_Len < other.m_Len ? other : *this;
    bit_pattern rv{shorterPattern};

    for (decltype(shorterPattern.m_RawBytes)::size_type i = 0; i < shorterPattern.m_RawBytes.size(); ++i)
        rv.m_RawBytes[i] = shorterPattern.m_RawBytes[i] ^ longerPattern.m_RawBytes[i];

    return rv;
}

pinepp::bit_pattern pinepp::bit_pattern::operator~() const {
    bit_pattern rv{*this};

    for (decltype(this->m_RawBytes)::size_type i = 0; i < this->m_RawBytes.size(); ++i)
        rv.m_RawBytes[i] = ~rv.m_RawBytes[i];

    return rv;
}


namespace pinepp {
    std::ostream &operator<<(std::ostream &os, const bit_pattern &pattern) {
        if (pattern.m_Len == 0)
            return os;

        for (size_t i = pattern.m_Len; i != 0; i--)
            os << ((pattern.m_RawBytes[(i - 1) / 8] & (1 << ((i - 1) % 8))) > 0 ? 1 : 0);

        return os;
    }
}


pinepp::bit_pattern::iterator::iterator(size_t index, const bit_pattern* ptr)
        : mp_BitPattern(ptr), m_Index(index) {}

pinepp::bit_pattern::iterator pinepp::bit_pattern::begin() const {
    return iterator{0, this};
}

pinepp::bit_pattern::iterator pinepp::bit_pattern::end() const {
    return iterator{m_Len, this};
}


pinepp::bit_pattern::iterator& pinepp::bit_pattern::iterator::operator++() {
    m_Index++;
    return *this;
}

bool pinepp::bit_pattern::iterator::operator==(pinepp::bit_pattern::iterator other) const {
    return this->m_Index == other.m_Index;
}

bool pinepp::bit_pattern::iterator::operator!=(pinepp::bit_pattern::iterator other) const {
    return this->m_Index != other.m_Index;
}

int pinepp::bit_pattern::iterator::operator*() const {
    return mp_BitPattern->m_RawBytes[m_Index / 8] & (1 << (m_Index % 8)) ? 1 : 0;
}