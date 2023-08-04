//
// Created by konstantin on 04.08.23.
//

#ifndef PINEPP_UTILITY_HPP
#define PINEPP_UTILITY_HPP

#include <iostream>
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
        friend std::ostream& operator<<(std::ostream &os, const pine_pp::uint128_t &value) {
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
}



#endif //PINEPP_UTILITY_HPP
