//
// Created by konstantin on 31.05.23.
//
#include <iostream>
#include "trie.hpp"
#include "gtest/gtest.h"
TEST(TrieInsert, OnlyAddsExactWord) {
    pinepp::trie<char> trie{};
    std::string a = "Hello";
    EXPECT_NO_THROW(trie.insert(a));
    EXPECT_TRUE(trie.contains("Hello"));
    EXPECT_FALSE(trie.contains("Hell"));
    EXPECT_FALSE(trie.contains("Hella"));
    EXPECT_FALSE(trie.contains("Hello!"));
    EXPECT_NO_THROW(trie.insert("Hello!"));
    EXPECT_TRUE(trie.contains("Hello!"));
    EXPECT_NO_THROW(trie.insert("Hell"));
    EXPECT_TRUE(trie.contains("Hell"));
    EXPECT_EQ(trie.size(),3);
}

TEST(StaticTrieInsert, OnlyAddsExactWord) {
    pinepp::static_trie<char> trie{5, "Helo!"};
    std::string a = "Hello";
    EXPECT_NO_THROW(trie.insert(a));
    EXPECT_TRUE(trie.contains("Hello"));
    EXPECT_FALSE(trie.contains("Hell"));
    EXPECT_FALSE(trie.contains("Hella"));
    EXPECT_FALSE(trie.contains("Hello!"));
    EXPECT_ANY_THROW(trie.insert("Hello!"));
    EXPECT_FALSE(trie.contains("Hello!"));
    EXPECT_ANY_THROW(trie.insert("Hell"));
    EXPECT_FALSE(trie.contains("Hell"));
    EXPECT_EQ(trie.size(),1);
}