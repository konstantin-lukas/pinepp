//
// Created by konstantin on 25.06.23.
//

#ifndef PINE_CONCEPTS_H
#define PINE_CONCEPTS_H
#include <iterator>
namespace pine_pp {
    template <typename T>
    concept iterable = requires(T t) {
        std::begin(t);
        std::end(t);
    };
    template <typename T>
    concept char_type = std::is_same_v<T, char> ||
                        std::is_same_v<T, wchar_t> ||
                        std::is_same_v<T, char8_t> ||
                        std::is_same_v<T, char16_t> ||
                        std::is_same_v<T, char32_t>;
}

#endif //PINE_CONCEPTS_H
