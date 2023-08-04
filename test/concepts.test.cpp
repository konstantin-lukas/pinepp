//
// Created by konstantin on 04.08.23.
//
#include <vector>
#include <array>
#include "gtest/gtest.h"
#include "concepts.hpp"
TEST(IterableConcept, MatchesOnlyWithTypesThatAreIterable) {
    using namespace pine_pp;
    static_assert(iterable<std::string>);
    static_assert(iterable<std::vector<int>>);
    static_assert(iterable<std::array<int,2>>);
}
TEST(PrintableIterableConcept, MatchesOnlyWithTypesThatAreIterable) {
    using namespace pine_pp;
    static_assert(printable_iterable<std::string>);
    static_assert(printable_iterable<std::vector<int>>);
    static_assert(printable_iterable<std::array<int,2>>);
}
TEST(CharTypeConcept, MatchesOnlyWithTypesThatAreCharTypes) {
    using namespace pine_pp;
    static_assert(char_type<char>);
    static_assert(char_type<wchar_t>);
    static_assert(char_type<char8_t>);
    static_assert(char_type<char16_t>);
    static_assert(char_type<char32_t>);
}