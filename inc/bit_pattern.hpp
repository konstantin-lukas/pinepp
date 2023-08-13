//
// Created by konstantin on 31.05.23.
//

#ifndef PINEPP_BIT_PATTERN_HPP
#define PINEPP_BIT_PATTERN_HPP
#include <cstddef>
#include <ostream>
#include <vector>
namespace pinepp {
    class bit_pattern {
    public:
        /**
         * @details Default constructs a bit_pattern with a size of 0
         */
        bit_pattern();

        /**
         * @details
         * Constructs a new bit pattern from a string representing bits or a hexadecimal number. Note that the right most
         * characters are interpreted as the least significant bits. This is only relevant when iterating over the pattern.
         * @param str
         */
        explicit bit_pattern(const std::string& str);

        /**
         * @details
         * Copy constructor for the bit_pattern class.
         * @param other The bit_pattern to copy
         */
        bit_pattern(const bit_pattern& other) = default;

        /**
         * @details
         * Move constructor for the bit_pattern class. Leaves the moved from object in an undefined but valid state.
         * @param other The bit_pattern to move from
         */
        bit_pattern(bit_pattern&& other) noexcept;

        /**
         * @returns The amount of bits in the pattern. This does not correspond to amount of memory used.
         */
        [[nodiscard]] size_t size() const;

        /**
         * @details
         * Sets the bit at position \p m_Index to 1 if \p value is true or 0 otherwise.
         * @param index Position of the bit to modify
         * @param value Value of the bit to modify
         */
        void set_bit(int index, bool value);

        /**
         * @details Reverses the bit pattern.
         */
        void reverse();

        class iterator {
        public:
            explicit iterator(size_t index, const bit_pattern* ptr);
            iterator& operator++();
            bool operator==(iterator other) const;
            bool operator!=(iterator other) const;
            int operator*() const;
        private:
            const bit_pattern* mp_BitPattern;
            size_t m_Index = 0;
        };

        [[nodiscard]] iterator begin() const;
        [[nodiscard]] iterator end() const;

        /**
         * @details Allows read-only access to bits at a certain position.
         * @returns An integer that is either 0 or 1. This integer is a representation of the bit in the array
         * and does not have the same address.
         */
        int operator[](unsigned int) const;

        /**
         * @returns The result of an AND operation on both patterns with the length of the shorter pattern
         */
        bit_pattern operator&(const bit_pattern& other) const;

        /**
         * @returns The result of an OR operation on both patterns with the length of the shorter pattern
         */
        bit_pattern operator|(const bit_pattern& other) const;

        /**
         * @returns The result of an XOR operation on both patterns with the length of the shorter pattern
         */
        bit_pattern operator^(const bit_pattern& other) const;

        /**
         * @returns The result of a NOT operation on the pattern
         */
        bit_pattern operator~() const;

        /**
         * @returns Shifts the pattern left \p n bits. Copies the overflowing bits to the other side.
         */
        bit_pattern operator<<(uint64_t n) const;

        /**
         * @returns Shifts the pattern right \p n bits. Copies the overflowing bits to the other side.
         */
        bit_pattern operator>>(uint64_t n) const;

        /**
         * @returns Concatenates two patterns
         */
        bit_pattern operator+(const bit_pattern& other) const;

        /**
         * @details Copy assignment operator of the bit_pattern class
         * @param other The pattern to copy from
         */
        bit_pattern& operator=(const bit_pattern& other);

        /**
         * @details Replaces the current pattern with the pattern represented by \p str.
         * @param str A string representing the new pattern to use
         */
        bit_pattern& operator=(const std::string& str);

        /**
         * @details Move assignment operator of the bit_pattern class
         * @param other The pattern to move from. Leaves the moved-from pattern in an undefined but valid state.
         */
        bit_pattern& operator=(bit_pattern&& other) noexcept;

        /**
         * @details Resizes the bit pattern. if \p b doesn't equal the current size, bits get either cut off or
         * padding bits with value 0 get inserted.
         * @param n The new size of the bit pattern.
         */
        bit_pattern& resize(size_t n);

        /**
         * @returns A string representing the bit pattern
         */
        [[nodiscard]] std::string str() const;
    private:
        std::vector<uint8_t> m_RawBytes{};
        std::size_t m_Len{0};
        friend std::ostream& operator<<(std::ostream& os, const bit_pattern& pattern);
        void from_string(const std::string& str);
    };
}

#endif //PINEPP_BIT_PATTERN_HPP
