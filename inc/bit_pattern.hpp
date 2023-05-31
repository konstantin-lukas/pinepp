//
// Created by konstantin on 31.05.23.
//

#ifndef PINE_BIT_PATTERN_HPP
#define PINE_BIT_PATTERN_HPP
#include <cstddef>
#include <ostream>
#include <vector>
namespace pine {
    class bit_pattern {
    public:
        bit_pattern() = delete;
        bit_pattern(std::initializer_list<bool> list);
        bit_pattern(std::initializer_list<int> list);
    private:
        std::vector<uint8_t> m_Bits{};
        std::size_t m_Len{0};
        friend std::ostream& operator<<(std::ostream& os, const bit_pattern& pattern);
    };
}

#endif //PINE_BIT_PATTERN_HPP
