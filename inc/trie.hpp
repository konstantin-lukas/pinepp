//
// Created by konstantin on 25.06.23.
//

#ifndef PINE_TRIE_HPP
#define PINE_TRIE_HPP
#include <cstddef>
#include <unordered_map>
#include <memory>
#include "concepts.hpp"
namespace pine_pp {
    template <char_type T = char>
    class trie {
    public:
        trie() : m_Size(0), m_Root(std::make_unique<s_Node>()) {};
        void insert(const std::basic_string<T>& string) {
            auto* node = m_Root.get();
            for (const auto& c : string) {
                if (node->m_Children.find(c) == node->m_Children.end())
                    node->m_Children[c] = c == string.back() ? std::make_unique<s_Final>(): std::make_unique<s_Node>();
                node = node->m_Children[c].get();
            }
        }
        bool contains(const std::basic_string<T>& string) {
            auto* node = m_Root.get();
            for (const auto& c : string) {
                if (node->m_Children.find(c) == node->m_Children.end())
                    return false;
                node = node->m_Children[c].get();
            }
            return dynamic_cast<s_Final*>(node) != nullptr;
        }
    private:
        struct s_Node {
            std::unordered_map<T, std::unique_ptr<s_Node>> m_Children;
            virtual ~s_Node() = default;
        };
        struct s_Final : public s_Node {};
        std::size_t m_Size;
        std::unique_ptr<s_Node> m_Root;
    };
}



#endif //PINE_TRIE_HPP
