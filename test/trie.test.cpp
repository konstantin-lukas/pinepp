//
// Created by konstantin on 31.05.23.
//
#include <iostream>
#include "trie.hpp"
#include "gtest/gtest.h"

template <typename CharT>
class TrieTest : public testing::Test {};

using CharTypes = testing::Types<char, wchar_t, char8_t, char16_t, char32_t>;
TYPED_TEST_SUITE(TrieTest, CharTypes);

TYPED_TEST(TrieTest, OnlyAddsExactWord) {
    using CharT = TypeParam;
    std::basic_string<CharT> a,b,c,d;
    if constexpr (std::same_as<CharT, char>) {
        a = "Hello";
        b = "Hell";
        c = "Hella";
        d = "Hello!";
    } else if constexpr (std::same_as<CharT, wchar_t>) {
        a = L"Hello";
        b = L"Hell";
        c = L"Hella";
        d = L"Hello!";
    } else if constexpr (std::same_as<CharT, char8_t>) {
        a = u8"Hello";
        b = u8"Hell";
        c = u8"Hella";
        d = u8"Hello!";
    } else if constexpr (std::same_as<CharT, char16_t>) {
        a = u"Hello";
        b = u"Hell";
        c = u"Hella";
        d = u"Hello!";
    } else if constexpr (std::same_as<CharT, char32_t>) {
        a = U"Hello";
        b = U"Hell";
        c = U"Hella";
        d = U"Hello!";
    }
    pinepp::basic_trie<CharT> trie{};
    EXPECT_NO_THROW(trie.insert(a));
    EXPECT_TRUE(trie.contains(a));
    EXPECT_FALSE(trie.contains(b));
    EXPECT_FALSE(trie.contains(c));
    EXPECT_FALSE(trie.contains(d));
    EXPECT_NO_THROW(trie.insert(d));
    EXPECT_TRUE(trie.contains(d));
    EXPECT_NO_THROW(trie.insert(b));
    EXPECT_TRUE(trie.contains(b));
    EXPECT_EQ(trie.size(), 3);
}

TYPED_TEST(TrieTest, WorksWithEmptyString) {
    using CharT = TypeParam;
    std::basic_string<CharT> a{};
    pinepp::basic_trie<CharT> trie{};
    EXPECT_FALSE(trie.contains(a));
    EXPECT_NO_THROW(trie.insert(a));
    EXPECT_TRUE(trie.contains(a));
}


template <typename CharT>
class StaticTrieTest : public testing::Test {};

TYPED_TEST_SUITE(StaticTrieTest, CharTypes);

TYPED_TEST(StaticTrieTest, OnlyAddsExactWord) {
    using CharT = TypeParam;
    std::basic_string<CharT> a,b,c,d,alphabet;
    if constexpr (std::same_as<CharT, char>) {
        a = "Hello";
        b = "Hell";
        c = "Hella";
        d = "Hello!";
        alphabet = "Helo!";
    } else if constexpr (std::same_as<CharT, wchar_t>) {
        a = L"Hello";
        b = L"Hell";
        c = L"Hella";
        d = L"Hello!";
        alphabet = L"Helo!";
    } else if constexpr (std::same_as<CharT, char8_t>) {
        a = u8"Hello";
        b = u8"Hell";
        c = u8"Hella";
        d = u8"Hello!";
        alphabet = u8"Helo!";
    } else if constexpr (std::same_as<CharT, char16_t>) {
        a = u"Hello";
        b = u"Hell";
        c = u"Hella";
        d = u"Hello!";
        alphabet = u"Helo!";
    } else if constexpr (std::same_as<CharT, char32_t>) {
        a = U"Hello";
        b = U"Hell";
        c = U"Hella";
        d = U"Hello!";
        alphabet = U"Helo!";
    }
    pinepp::basic_static_trie<CharT> trie{5, alphabet};
    EXPECT_NO_THROW(trie.insert(a));
    EXPECT_TRUE(trie.contains(a));
    EXPECT_FALSE(trie.contains(b));
    EXPECT_FALSE(trie.contains(c));
    EXPECT_FALSE(trie.contains(d));
    EXPECT_ANY_THROW(trie.insert(d));
    EXPECT_FALSE(trie.contains(d));
    EXPECT_ANY_THROW(trie.insert(b));
    EXPECT_FALSE(trie.contains(b));
    EXPECT_EQ(trie.size(), 1);
}

TYPED_TEST(StaticTrieTest, RefusesToConstructWithInvalidArguments) {
    using CharT = TypeParam;
    std::basic_string<CharT> empty,alphabet;
    if constexpr (std::same_as<CharT, char>) {
        alphabet = "Helo!";
        empty = "";
    } else if constexpr (std::same_as<CharT, wchar_t>) {
        alphabet = L"Helo!";
        empty = L"";
    } else if constexpr (std::same_as<CharT, char8_t>) {
        alphabet = u8"Helo!";
        empty = u8"";
    } else if constexpr (std::same_as<CharT, char16_t>) {
        alphabet = u"Helo!";
        empty = u"";
    } else if constexpr (std::same_as<CharT, char32_t>) {
        alphabet = U"Helo!";
        empty = U"";
    }
    EXPECT_ANY_THROW(pinepp::basic_static_trie<CharT>(0, empty));
    EXPECT_ANY_THROW(pinepp::basic_static_trie<CharT>(0, alphabet));
    EXPECT_ANY_THROW(pinepp::basic_static_trie<CharT>(3, empty));
}