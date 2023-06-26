//
// Created by konstantin on 31.05.23.
//
#include <iostream>
#include "trie.hpp"
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
    trie.insert("Hell");
    std::cout << trie.contains("Hell") << '\n';
    std::cout << trie.size() << '\n';
    std::cout << "-----------------STATIC TRIE-----------------" << '\n';

    pine_pp::static_trie<char> staticTrie{5,"Helo!"};
    staticTrie.insert(a);
    std::cout << staticTrie.contains("Hello") << '\n';
    std::cout << staticTrie.contains("Hell") << '\n';
    std::cout << staticTrie.contains("Hella") << '\n';
    std::cout << staticTrie.contains("Hell!") << '\n';
    staticTrie.insert("Hell!");
    std::cout << staticTrie.contains("Hell!") << '\n';
    staticTrie.insert("olleH");
    std::cout << staticTrie.contains("olleH") << '\n';
    staticTrie.insert("Hell!");
    std::cout << staticTrie.contains("Hell") << '\n';
    std::cout << staticTrie.size() << '\n';
}