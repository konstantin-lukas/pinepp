//
// Created by konstantin on 31.05.23.
//
#include <iostream>
#include "base64.hpp"
#include "trie.hpp"
#include "bit_pattern.hpp"
int main() {

    pine_pp::trie<char> trie{};
    std::string a = "Hello";
    trie.insert(a);

    std::cout << trie.contains("Hello") << '\n';
    std::cout << trie.contains("Hell") << '\n';
    std::cout << trie.contains("Hella") << '\n';
    std::cout << trie.contains("Hello!") << '\n';
    trie.insert("Hello!");
    std::cout << trie.contains("Hello!") << '\n';
    trie.insert("Hello!");
    std::cout << trie.contains("Hello!") << '\n';
}