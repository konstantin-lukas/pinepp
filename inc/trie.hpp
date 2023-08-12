//
// Created by konstantin on 25.06.23.
//

#ifndef PINEPP_TRIE_HPP
#define PINEPP_TRIE_HPP
#include <cstddef>
#include <unordered_map>
#include <memory>
#include <vector>
#include <unordered_set>
#include "concepts.hpp"
namespace pinepp {

    /**
     * @details A basic_trie is a structure that allows for constant time lookup and insertion of strings
     * (linear in the size of the inserted/searched string. Its worst case space complexity is very high however.
     * It has O(n*k) space complexity where n is the amount of internal nodes
     * (one for each unique character inserted in one position) and k is the size of the alphabet.
     * @tparam T The char type to determine the kind of string to use,
     * e.g. char for std::basic_string<char> (which is std::string).
     */
    template <char_type T = char>
    // TODO: COPY/MOVE SEMANTICS
    // TODO: DELETER (NO SMART PTRS)
    // TODO: INITIALIZER LIST CONSTRUCTOR
    // TODO: LONGEST PREFIX MATCH
    class basic_trie {
    public:
        constexpr basic_trie() : m_Size(0), m_Root(std::make_unique<s_Node>()) {};
        constexpr void insert(const std::basic_string<T>& string) {
            auto* node = m_Root.get();
            bool isNew = false;
            for (const auto& c : string) {
                if (node->m_Children.find(c) == node->m_Children.end()) {
                    node->m_Children.emplace(c, std::make_unique<s_Node>());
                    isNew = true;
                }
                node = node->m_Children[c].get();
            }
            if (isNew || !node->m_IsFinal) {
                m_Size++;
                node->m_IsFinal = true;
            }
        }
        constexpr bool contains(const std::basic_string<T>& string) const {
            auto* node = m_Root.get();
            for (const auto& c : string) {
                if (node->m_Children.find(c) == node->m_Children.end())
                    return false;
                node = node->m_Children[c].get();
            }
            return node->m_IsFinal;
        }
        [[nodiscard]] constexpr std::size_t size() {
            return m_Size;
        }
        [[nodiscard]] constexpr std::size_t length() {
            return m_Size;
        }
    private:
        struct s_Node {
            std::unordered_map<T, std::unique_ptr<s_Node>> m_Children;
            bool m_IsFinal = false;
        };
        std::size_t m_Size;
        std::unique_ptr<s_Node> m_Root;
    };


    /**
     * @details A basic_static_trie is very similar to a basic_trie with two major differences.
     * 1. It has a fixed alphabet
     * 2. It has a fixed word length
     * While these constraints do not affect time complexity, they allow for better space complexity resulting in
     * O(n*k) where n is the word length and k is the alphabet size.
     * @tparam T The char type to determine the kind of string to use,
     * e.g. char for std::basic_string<char> (which is std::string).
     */
    template <char_type T>
    class basic_static_trie {
    public:
        constexpr basic_static_trie(const std::size_t wordLength, const std::basic_string<T>& alphabet) :
        m_Alphabet(alphabet),
        m_WordLength(wordLength),
        m_Size(0),
        m_Root(new T*[m_Alphabet.size()]{nullptr}) {
            if (wordLength == 0)
                throw std::invalid_argument("Word length has to be at least one");
            if (alphabet.size() == 0)
                throw std::invalid_argument("Alphabet length has to be at least one");
            std::unordered_set<T> seenCharacters;
            for (char c : alphabet) {
                if (seenCharacters.contains(c))
                    throw std::invalid_argument("Alphabet may only contain unique characters");
                seenCharacters.insert(c);
            }
        };
        constexpr void insert(const std::basic_string<T>& string) {
            if (string.size() != m_WordLength)
                throw std::length_error{"Length of string does not match word size of trie."};
            for (const auto& c : string) {
                if (m_Alphabet.find(c) == std::string::npos)
                    throw std::domain_error{"String contains characters that are not in the trie's alphabet."};
            }
            auto node = m_Root;
            bool isNew = false;
            for (const auto& c : string) {
                auto index = m_Alphabet.find(c);
                if (node[index] == nullptr) {
                    node[index] = reinterpret_cast<T*>(new T*[m_Alphabet.size()]{nullptr});
                    isNew = true;
                }
                node = reinterpret_cast<T**>(node[index]);
            }
            if (isNew)
                m_Size++;
        }
        constexpr bool contains(const std::basic_string<T>& string) const {
            if (string.size() != m_WordLength)
                return false;
            auto node = m_Root;
            for (const auto& c : string) {
                auto index = m_Alphabet.find(c);
                if (index == std::string::npos || node[index] == nullptr)
                    return false;

                node = reinterpret_cast<T**>(node[index]);
            }
            return true;
        }
        [[nodiscard]] constexpr std::size_t size() {
            return m_Size;
        }
        [[nodiscard]] constexpr std::size_t length() {
            return m_Size;
        }
    private:
        const std::basic_string<T> m_Alphabet;
        const std::size_t m_WordLength;
        std::size_t m_Size;
        T** m_Root;
    };

    typedef basic_trie<char> trie;
    typedef basic_static_trie<char> static_trie;
    typedef basic_trie<wchar_t> wtrie;
    typedef basic_static_trie<wchar_t> wstatic_trie;
    typedef basic_trie<char8_t> u8trie;
    typedef basic_static_trie<char8_t> u8static_trie;
    typedef basic_trie<char16_t> u16trie;
    typedef basic_static_trie<char16_t> u16static_trie;
    typedef basic_trie<char32_t> u32trie;
    typedef basic_static_trie<char32_t> u32static_trie;
}



#endif //PINEPP_TRIE_HPP
