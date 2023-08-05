//
// Created by konstantin on 04.08.23.
//

#ifndef PINEPP_UTILITY_HPP
#define PINEPP_UTILITY_HPP

#include <iostream>
#include <iterator>
#include "concepts.hpp"
namespace pinepp {
    template<typename T>
    class is_class {
    private:
        template<typename S> constexpr static bool test(int S::*) { return true; }
        template<typename> constexpr static bool test(...) { return false; }
    public:
        constexpr static bool value = test<T>(0);
    };

    template <printable_iterable T>
    std::ostream& print_iterable (const T& iterable, std::ostream& os) {
        os << '[';
        const std::string separator{", "};
        for (auto it = std::begin(iterable); it != std::end(iterable); ++it) {
            if (it != std::begin(iterable))
                os << separator;
            os << *it;
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
