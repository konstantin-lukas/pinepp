//
// Created by konstantin on 04.08.23.
//

#ifndef PINEPP_UTILITY_HPP
#define PINEPP_UTILITY_HPP

#include <iostream>
#include <iterator>
#include "concepts.hpp"
namespace pinepp {
    /**
     * @brief Utility class to check if a type \p T is a class type or a primitive type
     * @tparam T
     */
    template<typename T>
    class is_class {
    private:
        template<typename S> constexpr static bool test(int S::*) { return true; }
        template<typename> constexpr static bool test(...) { return false; }
    public:
        constexpr static bool value = test<T>(0);
    };

    /**
     * @details Write an iterable container to an ostream like an array, e.g. [1, 5, ..., 6].
     * @tparam T The type of element contained in the iterable container
     * @param iterable An iterable container
     * @param os The stream to write to
     * @return \p os
     */
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

    /**
     * @details Print an iterable container to std::cout like an array, e.g. [1, 5, ..., 6].
     * @tparam T The type of element contained in the iterable container
     * @param iterable An iterable container
     */
    template <printable_iterable T>
    void print_iterable (const T& iterable) {
        print_iterable<T>(iterable, std::cout);
    }

    /**
     * @brief Enum class representing tools that can be used to make http requests
     */
    enum class http_tools : uint8_t { CURL, WGET };

    /**
     * @details Fetches a resource from a URL by making an HTTP request and saves it as a file.
     * @param url The URL to fetch.
     * @param filepath The location where to save the file.
     */
    void fetch(const std::string& url, const std::string& filepath, http_tools tool = http_tools::CURL);

    /**
     * @details Fetches a resource from a URL by making an HTTP request.
     * @param url The URL to fetch.
     * @param tool The tool to use to make the http request. Defaults to cURL.
     */
    std::string fetch(const std::string& url, http_tools tool = http_tools::CURL);
}



#endif //PINEPP_UTILITY_HPP
