//
// Created by konstantin on 04.08.23.
//

#ifndef PINEPP_UTILITY_HPP
#define PINEPP_UTILITY_HPP

#include <iostream>
#include <iterator>
#include "concepts.hpp"
namespace pine_pp {
    template<typename T>
    class is_class {
    private:
        template<typename S> constexpr static bool test(int S::*) { return true; }
        template<typename> constexpr static bool test(...) { return false; }
    public:
        constexpr static bool value = test<T>(0);
    };

    typedef struct uint128_t {
    public:
        explicit uint128_t(uint64_t value) : m_First(value), m_Second(0) {}
        friend std::ostream& operator<<(std::ostream& os, const pine_pp::uint128_t& value) {
            if (value.m_Second != 0) {
                os << value.m_Second;
                os << value.m_First;
            } else {
                os << value.m_First;
            }

            return os;
        }
    private:
        uint64_t m_First;
        uint64_t m_Second;
    } uint128_t;

    template <printable_iterable T>
    std::ostream& print_iterable (const T& iterable, std::ostream& os) {
        os << '[';
        const std::string separator{", "};
        for (const auto& element : iterable) {
            os << element;
            if (&element != &iterable.back())
                os << separator;
        }
        os << ']';
        return os;
    }

    template <printable_iterable T>
    void print_iterable (const T& iterable) {
        print_iterable<T>(iterable, std::cout);
    }
}



#endif //PINEPP_UTILITY_HPP
