//
// Created by konstantin on 25.06.23.
//

#ifndef PINEPP_CONCEPTS_H
#define PINEPP_CONCEPTS_H
#include <iterator>
namespace pinepp {
    template <typename T>
    concept iterable = requires(T t) {
        std::begin(t);
        std::end(t);
    };
    template <typename T>
    concept printable_iterable = requires(T t) {
        std::begin(t);
        std::end(t);
        std::cout << *std::begin(t);
    };
    template <typename T>
    concept char_type = std::is_same_v<T, char> ||
                        std::is_same_v<T, wchar_t> ||
                        std::is_same_v<T, char8_t> ||
                        std::is_same_v<T, char16_t> ||
                        std::is_same_v<T, char32_t>;
}

#endif //PINEPP_CONCEPTS_H
