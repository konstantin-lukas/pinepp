//
// Created by konstantin on 31.05.23.
//

#include <iostream>
#include <ranges>
#include <regex>
#include "bit_pattern.hpp"


pinepp::bit_pattern::bit_pattern() : m_Len(0) {}

pinepp::bit_pattern::bit_pattern(const std::string& str) {
    from_string(str);
}

void pinepp::bit_pattern::from_string(const std::string& str) {
    m_RawBytes.clear();
    if (std::regex pattern{"^[01]+$"}; std::regex_match(str, pattern)) {
        m_Len = str.size();
        m_RawBytes.resize(((m_Len - 1) / 8) + 1, 0);
        int counter = 0;
        for (auto c : std::ranges::reverse_view(str)) {
            if (c == '1')
                m_RawBytes[counter / 8] |= (1 << (counter % 8));
            counter++;
        }
    } else if (pattern = "^0[xX][0-9a-fA-F]+$"; std::regex_match(str, pattern)) {
        m_Len = (str.size() - 2) * 4;
        m_RawBytes.resize((m_Len - 1 / 8) + 1, 0);
        size_t counter = 0;
        auto it = str.rbegin();
        while (counter < str.size()) {
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
    auto c = this->str();
    std::reverse(c.begin(), c.end());
    *this = bit_pattern{c};
}

pinepp::bit_pattern& pinepp::bit_pattern::operator=(const pinepp::bit_pattern& other) {
    if (&other == this)
        return *this;
    this->m_Len = other.m_Len;
    this->m_RawBytes = other.m_RawBytes;
    return *this;
}

pinepp::bit_pattern& pinepp::bit_pattern::operator=(const std::string& str) {
    from_string(str);
    return *this;
}

pinepp::bit_pattern& pinepp::bit_pattern::operator=(pinepp::bit_pattern&& other) noexcept {
    if (&other == this)
        return *this;
    this->m_Len = other.m_Len;
    this->m_RawBytes = std::move(other.m_RawBytes);
    other.m_Len = 0;
    return *this;
}

int pinepp::bit_pattern::operator[](unsigned int index) const {
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

std::string pinepp::bit_pattern::str() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

bool pinepp::bit_pattern::operator==(const pinepp::bit_pattern &other) const noexcept {
    if (this->size() != other.size())
        return false;
    if (this == &other)
        return true;
    auto it1 = this->begin();
    auto it2 = other.begin();
    while (it1 != this->end() && it2 != other.end()) {
        if (*it1 != *it2)
            return false;
        ++it1;
        ++it2;
    }
    return true;
}

bool pinepp::bit_pattern::operator!=(const pinepp::bit_pattern &other) const noexcept {
    return !(*this == other);
}

pinepp::bit_pattern& pinepp::bit_pattern::resize(size_t n) {
    if (m_Len == n)
        return *this;
    else if (std::string str = this->str(); m_Len > n) {
        from_string(str.substr(str.size() - n));
    } else {
        from_string(std::string(n - str.size(), '0') + str);
    }
    return *this;
}

pinepp::bit_pattern pinepp::bit_pattern::operator<<(uint64_t n) const {
    uint64_t shift = n % m_Len;
    if (m_Len <= 1 || shift == 0)
        return *this;
    auto str = this->str();
    std::string firstPart = str.substr(0, shift);
    std::string secondPart = str.substr(shift);
    bit_pattern rv{secondPart + firstPart};
    return rv;
}

pinepp::bit_pattern pinepp::bit_pattern::operator>>(uint64_t n) const {
    uint64_t shift = n % m_Len;
    if (m_Len <= 1 || shift == 0)
        return *this;
    auto str = this->str();
    std::string firstPart = str.substr(0, str.size() - shift);
    std::string secondPart = str.substr(str.size() - shift);
    bit_pattern rv{secondPart + firstPart};
    return rv;
}


pinepp::bit_pattern pinepp::bit_pattern::operator+(const bit_pattern& other) const {
    if (this->m_Len > 0 && other.m_Len > 0) {
        return bit_pattern{this->str() + other.str()};
    } else if (this->m_Len == 0 && other.m_Len > 0)
        return other;
    else if (this->m_Len > 0 && other.m_Len == 0)
        return *this;
    else
        return bit_pattern{};
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