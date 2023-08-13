//
// Created by konstantin on 31.05.23.
//
#include <iostream>
#include "trie.hpp"
#include "gtest/gtest.h"

template <typename CharT>
class TrieTest : public testing::Test {
public:
    std::basic_string<CharT> a,b,c,d,e;
    void SetUp() override {
        if constexpr (std::same_as<CharT, char>) {
            a = "Hello";
            b = "Hell";
            c = "Hella";
            d = "Hello!";
            e = "";
        } else if constexpr (std::same_as<CharT, wchar_t>) {
            a = L"Hello";
            b = L"Hell";
            c = L"Hella";
            d = L"Hello!";
            e = L"";
        } else if constexpr (std::same_as<CharT, char8_t>) {
            a = u8"Hello";
            b = u8"Hell";
            c = u8"Hella";
            d = u8"Hello!";
            e = u8"";
        } else if constexpr (std::same_as<CharT, char16_t>) {
            a = u"Hello";
            b = u"Hell";
            c = u"Hella";
            d = u"Hello!";
            e = u"";
        } else if constexpr (std::same_as<CharT, char32_t>) {
            a = U"こんにちは";
            b = U"こんにち";
            c = U"こんにちわ";
            d = U"こんにちは！";
            e = U"";
        }
    }
};

using CharTypes = testing::Types<char, wchar_t, char8_t, char16_t, char32_t>;
TYPED_TEST_SUITE(TrieTest, CharTypes);

TYPED_TEST(TrieTest, OnlyAddsExactWord) {
    pinepp::basic_trie<TypeParam> trie{};
    EXPECT_NO_THROW(trie.insert(this->a));
    EXPECT_TRUE(trie.contains(this->a));
    EXPECT_FALSE(trie.contains(this->b));
    EXPECT_FALSE(trie.contains(this->c));
    EXPECT_FALSE(trie.contains(this->d));
    EXPECT_NO_THROW(trie.insert(this->d));
    EXPECT_TRUE(trie.contains(this->d));
    EXPECT_NO_THROW(trie.insert(this->b));
    EXPECT_TRUE(trie.contains(this->b));
    EXPECT_EQ(trie.size(), 3);
}

TYPED_TEST(TrieTest, TestsInitializerListConstructor) {
    pinepp::basic_trie<TypeParam> trie{this->a,this->b,this->c};
    EXPECT_TRUE(trie.contains(this->a));
    EXPECT_TRUE(trie.contains(this->b));
    EXPECT_TRUE(trie.contains(this->c));
    EXPECT_FALSE(trie.contains(this->d));
    EXPECT_EQ(trie.size(), 3);
}

TYPED_TEST(TrieTest, WorksWithEmptyString) {
    pinepp::basic_trie<TypeParam> trie{};
    EXPECT_FALSE(trie.contains(this->e));
    EXPECT_NO_THROW(trie.insert(this->e));
    EXPECT_TRUE(trie.contains(this->e));
}

TYPED_TEST(TrieTest, LongestPrefix) {
    pinepp::basic_trie<TypeParam> trie{};
    EXPECT_EQ(trie.longest_prefix(this->c),0);
    EXPECT_NO_THROW(trie.insert(this->e));
    EXPECT_EQ(trie.longest_prefix(this->c),0);
    EXPECT_NO_THROW(trie.insert(this->a));
    EXPECT_EQ(trie.longest_prefix(this->c),4);
}
/*
TYPED_TEST(TrieTest, LetsYouIterateOverWordsInTrie) {
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
        a = U"こんにちは";
        b = U"こんにち";
        c = U"こんにちわ";
        d = U"こんにちは！";
    }
    std::unordered_set<std::basic_string<CharT>> words{};
    words.insert(a);
    words.insert(b);
    words.insert(c);
    words.insert(d);
    pinepp::basic_trie<CharT> trie{};
    EXPECT_NO_THROW(trie.insert(a));
    EXPECT_NO_THROW(trie.insert(b));
    EXPECT_NO_THROW(trie.insert(c));
    EXPECT_NO_THROW(trie.insert(d));
    size_t count = 0;
    for (const auto& word : trie) {
        EXPECT_TRUE(words.contains(word));
        count++;
    }
    EXPECT_EQ(trie.size(), count);
    EXPECT_EQ(words.size(), count);
}*/
/*
TYPED_TEST(TrieTest, CopyConstructor) {
    using CharT = TypeParam;
    std::basic_string<CharT> a,b;
    if constexpr (std::same_as<CharT, char>) {
        a = "Hello";
        b = "Goodbye";
    } else if constexpr (std::same_as<CharT, wchar_t>) {
        a = L"Hello";
        b = L"Goodbye";
    } else if constexpr (std::same_as<CharT, char8_t>) {
        a = u8"Hello";
        b = u8"Goodbye";
    } else if constexpr (std::same_as<CharT, char16_t>) {
        a = u"Straße";
        b = u"Goodbye";
    } else if constexpr (std::same_as<CharT, char32_t>) {
        a = U"こんにちは";
        b = U"さようなら";
    }
    pinepp::basic_trie<CharT> trie{};
    EXPECT_NO_THROW(trie.insert(a));
    pinepp::basic_trie<CharT>copy{trie};
    EXPECT_TRUE(copy.contains(a));
    EXPECT_FALSE(copy.contains(b));
}
*/

template <typename CharT>
class StaticTrieTest : public testing::Test {
public:
    std::basic_string<CharT> a,b,c,d,e,f,alphabet;
    void SetUp() override {
        if constexpr (std::same_as<CharT, char>) {
            a = "Hello";
            b = "Hell";
            c = "Hella";
            d = "Hello!";
            e = "";
            alphabet = "Helo!Dank";
            f = "Danke";
        } else if constexpr (std::same_as<CharT, wchar_t>) {
            a = L"Hello";
            b = L"Hell";
            c = L"Hella";
            d = L"Hello!";
            e = L"";
            f = L"Danke";
            alphabet = L"Helo!Dank";
        } else if constexpr (std::same_as<CharT, char8_t>) {
            a = u8"Hello";
            b = u8"Hell";
            c = u8"Hella";
            d = u8"Hello!";
            e = u8"";
            f = u8"Danke";
            alphabet = u8"Helo!Dank";
        } else if constexpr (std::same_as<CharT, char16_t>) {
            a = u"Hello";
            b = u"Hell";
            c = u"Hella";
            d = u"Hello!";
            e = u"";
            f = u"Danke";
            alphabet = u"Helo!Dank";
        } else if constexpr (std::same_as<CharT, char32_t>) {
            a = U"こんにちは";
            b = U"こんにち";
            c = U"こんにちわ";
            d = U"こんにちは！";
            e = U"";
            f = U"ありがとう";
            alphabet = U"こんにちは！ありがとう";
        }
    }
};

TYPED_TEST_SUITE(StaticTrieTest, CharTypes);

TYPED_TEST(StaticTrieTest, OnlyAddsExactWord) {
    pinepp::basic_static_trie<TypeParam> trie{5, this->alphabet};
    EXPECT_NO_THROW(trie.insert(this->a));
    EXPECT_TRUE(trie.contains(this->a));
    EXPECT_FALSE(trie.contains(this->b));
    EXPECT_FALSE(trie.contains(this->c));
    EXPECT_FALSE(trie.contains(this->d));
    EXPECT_ANY_THROW(trie.insert(this->d));
    EXPECT_FALSE(trie.contains(this->d));
    EXPECT_ANY_THROW(trie.insert(this->b));
    EXPECT_FALSE(trie.contains(this->b));
    EXPECT_EQ(trie.size(), 1);
}

TYPED_TEST(StaticTrieTest, RefusesToConstructWithInvalidArguments) {
    EXPECT_ANY_THROW(pinepp::basic_static_trie<TypeParam>(0, this->e));
    EXPECT_ANY_THROW(pinepp::basic_static_trie<TypeParam>(0, this->alphabet));
    EXPECT_ANY_THROW(pinepp::basic_static_trie<TypeParam>(3, this->e));
}

TYPED_TEST(StaticTrieTest, TestsInitializerListConstructor) {
    pinepp::basic_static_trie<TypeParam> trie{5, this->alphabet, {this->a, this->f}};
    EXPECT_TRUE(trie.contains(this->a));
    EXPECT_FALSE(trie.contains(this->b));
    EXPECT_TRUE(trie.contains(this->f));
    EXPECT_FALSE(trie.contains(this->d));
    EXPECT_EQ(trie.size(), 2);
}

TYPED_TEST(StaticTrieTest, LongestPrefix) {
    pinepp::basic_static_trie<TypeParam> trie{5, this->alphabet};
    EXPECT_EQ(trie.longest_prefix(this->c),0);
    EXPECT_NO_THROW(trie.insert(this->a));
    EXPECT_EQ(trie.longest_prefix(this->c),4);
}