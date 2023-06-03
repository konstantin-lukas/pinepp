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
        /**
         * @details
         * Constructs a new bit pattern from a list of boolean values where true represents 1 and false represents 0. Note that
         * this is mostly intended for convenience. It is faster to construct a bit pattern from a list of uint8_t which matches
         * the internal data representation.
         * @param list
         */
        bit_pattern(std::initializer_list<bool> list);
        bit_pattern(std::initializer_list<int> list);
        [[nodiscard]] size_t size() const;

        class iterator {
        public:
            explicit iterator(size_t index, bit_pattern* ptr);
            iterator& operator++();
            bool operator==(iterator other) const;
            bool operator!=(iterator other) const;
            int operator*() const;
        private:
            bit_pattern* mp_BitPattern;
            size_t m_Index = 0;
        };

        iterator begin();
        iterator end();
        int operator[](int) const;
    private:
        std::vector<uint8_t> m_RawBytes{};
        std::size_t m_Len{0};
        friend std::ostream& operator<<(std::ostream& os, const bit_pattern& pattern);
    };
}

#endif //PINE_BIT_PATTERN_HPP
