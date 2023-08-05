//
// Created by konstantin on 25.06.23.
//

#ifndef PINEPP_TRIE_HPP
#define PINEPP_TRIE_HPP
#include <cstddef>
#include <unordered_map>
#include <memory>
#include <vector>
#include "concepts.hpp"
namespace pinepp {
    template <char_type T = char>
    // TODO: COPY/MOVE SEMANTICS
    // TODO: DELETER (NO SMART PTRS)
    // TODO: INITIALIZER LIST CONSTRUCTOR
    // TODO: LONGEST PREFIX MATCH
    class trie {
    public:
        constexpr trie() : m_Size(0), m_Root(std::make_unique<s_Node>()) {};
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

    template <char_type T>
    class static_trie {
    public:
        // TODO: TEST THAT ALPHABET CONTAINS ONLY UNIQUE CHARACTERS
        constexpr static_trie(const std::size_t wordLength, const std::basic_string<T>& alphabet) :
        m_Alphabet(alphabet),
        m_WordLength(wordLength),
        m_Size(0),
        m_Root(new T*[m_Alphabet.size()]{nullptr}) {};
        constexpr void insert(const std::basic_string<T>& string) {
            if (string.size() != m_WordLength)
                throw std::length_error{"Length of string does not match word size of static trie."};
            for (const auto& c : string) {
                if (m_Alphabet.find(c) == std::string::npos)
                    throw std::domain_error{"String contains characters that are not in the static trie alphabet."};
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
}



#endif //PINEPP_TRIE_HPP
