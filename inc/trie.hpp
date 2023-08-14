//
// Created by konstantin on 25.06.23.
//

#ifndef PINEPP_TRIE_HPP
#define PINEPP_TRIE_HPP
#include <cstddef>
#include <map>
#include <memory>
#include <vector>
#include <unordered_set>
#include <stack>
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
    // TODO: COPY/MOVE SEMANTICS
    template <char_type T = char>
    class basic_trie {
    private:
        struct s_Node {
            std::map<T, std::unique_ptr<s_Node>> m_Children;
            bool m_IsFinal = false;
        };
        std::size_t m_Size;
        std::unique_ptr<s_Node> m_Root;
        class iterator {
        public:

            iterator(const basic_trie<T>& trie, bool end) : m_Trie(trie) {
                m_NodeStack.push(m_Trie.m_Root.get());
                if (end) {
                    m_Index = m_Trie.size();
                } else {
                    m_Index = 0;
                    next_word();
                    m_Index = 0;
                }
            }

            iterator& operator++() {
                next_word();
                return *this;
            }

            /**
             * @returns A string representing the current word. Modifying this string has not effect on the trie.
             */
            std::basic_string<T> operator*() {
                return build_word();
            }

        private:
            class ArrowWrapper;
        public:
            /**
             * @return The string inside a wrapper class to allow access to address of temporary. Does not allow
             * modification of trie.
             */
            ArrowWrapper operator->() {
                return ArrowWrapper(build_word());
            }

            bool operator==(const iterator& other) const {
                return this->m_Index == other.m_Index;
            }

            bool operator!=(const iterator& other) const {
                return this->m_Index != other.m_Index;
            }

        private:
            /**
             * @brief Left hand side depth first traversal to find next element in trie.
             */
            void next_word() {
                if (this->m_Index == m_Trie.size()) {
                    // DONE ITERATING
                    return;
                }

                while (true) {
                    auto children = &m_NodeStack.top()->m_Children;
                    auto next = m_SymbolStack.empty() || m_SymbolStack.size() == m_NodeStack.size() - 1
                                ? children->begin()
                                : children->upper_bound(m_SymbolStack.top());
                    if (m_SymbolStack.size() > m_NodeStack.size() - 1)
                        m_SymbolStack.pop();
                    if (next == children->end()) {
                        if (m_NodeStack.top() == m_Trie.m_Root.get()) {
                            // DONE ITERATING
                            m_Index++;
                            return;
                        } else {
                            // BACKTRACK
                            m_NodeStack.pop();
                        }
                    } else {
                        // GO DEEPER
                        m_SymbolStack.push(next->first);
                        m_NodeStack.push(next->second.get());
                    }
                    // FOUND NEXT WORD
                    if (m_NodeStack.top()->m_IsFinal && m_NodeStack.size() - 1  == m_SymbolStack.size()) {
                        m_Index++;
                        return;
                    }
                }

            }

            /**
             * @brief Builds a word from the current symbol stack
             * @returns The Word built from the current symbol stack
             */
            std::basic_string<T> build_word() {
                std::stack<T> copy{m_SymbolStack};
                std::basic_string<T> rv{};
                rv.resize(copy.size());
                auto counter = copy.size() - 1;
                while (!copy.empty()) {
                    rv[counter] = copy.top();
                    copy.pop();
                    counter--;
                }
                return rv;
            }

            const basic_trie<T>& m_Trie;
            std::stack<s_Node*> m_NodeStack;
            std::stack<T> m_SymbolStack;
            size_t m_Index;
            class ArrowWrapper {
                const std::basic_string<T> value;
            public:
                explicit ArrowWrapper(std::basic_string<T> value) : value(value) {}
                const std::basic_string<T>* operator->() const {
                    return &value;
                }
            };
        };
    public:
        constexpr basic_trie() : m_Size(0), m_Root(std::make_unique<s_Node>()) {};
        constexpr basic_trie(std::initializer_list<std::basic_string<T>> words) {
            m_Size = 0;
            m_Root = std::make_unique<s_Node>();
            for (const auto& word : words) {
                insert(word);
            }
        };
        constexpr basic_trie(const basic_trie<T>& other) {
            m_Size = 0;
            m_Root = std::make_unique<s_Node>();
            for (const auto& word : other) {
                insert(word);
            }
        }
        constexpr basic_trie(basic_trie<T>&& other) noexcept {
            m_Size = other.m_Size;
            m_Root = std::move(other.m_Root);
            other.m_Size = 0;
            other.m_Root = std::make_unique<s_Node>();
        }
        constexpr basic_trie& operator=(const basic_trie& other) {
            if (this == &other)
                return *this;
            m_Size = 0;
            m_Root = std::make_unique<s_Node>();
            for (const auto& word : other) {
                insert(word);
            }
            return *this;
        }
        constexpr basic_trie& operator=(basic_trie&& other) noexcept {
            if (this == &other)
                return *this;
            m_Size = other.m_Size;
            m_Root = std::move(other.m_Root);
            other.m_Size = 0;
            other.m_Root = std::make_unique<s_Node>();
            return *this;
        }
        constexpr void insert(const std::basic_string<T>& string) {
            auto* node = m_Root.get();
            bool isNew = false;
            for (const auto& c : string) {
                if (!node->m_Children.contains(c)) {
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
        [[nodiscard]] constexpr bool contains(const std::basic_string<T>& string) const {
            auto* node = m_Root.get();
            for (const auto& c : string) {
                if (!node->m_Children.contains(c))
                    return false;
                node = node->m_Children[c].get();
            }
            return node->m_IsFinal;
        }
        [[nodiscard]] constexpr std::size_t size() const {
            return m_Size;
        }
        [[nodiscard]] constexpr std::size_t length() const {
            return m_Size;
        }
        [[nodiscard]] constexpr int longest_prefix(const std::basic_string<T>& string) const {
            auto* node = m_Root.get();
            int count = 0;
            for (const auto c : string) {
                if (node->m_Children.contains(c)) {
                    count++;
                    node = node->m_Children[c].get();
                } else {
                    break;
                }
            }
            return count;
        }

        [[nodiscard]] iterator begin() const {
            return iterator{*this, false};
        }

        [[nodiscard]] iterator end() const {
            return iterator{*this, true};
        }
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

    private:
        void recursive_delete(T** node) {
            for (decltype(m_Alphabet.size()) i = 0; i < m_Alphabet.size(); ++i) {
                if (node[i])
                    recursive_delete((T**)node[i]);
            }
            delete[] node;
        }
        std::basic_string<T> m_Alphabet;
        std::size_t m_WordLength;
        std::size_t m_Size;
        T** m_Root;
        class iterator {
        public:

            iterator(const basic_static_trie<T>& trie, bool end) : m_Trie(trie) {
                m_NodeStack.push(m_Trie.m_Root);
                if (end) {
                    m_Index = m_Trie.size();
                } else {
                    m_Index = 0;
                    next_word();
                    m_Index = 0;
                }
            }

            iterator& operator++() {
                next_word();
                return *this;
            }

            /**
             * @returns A string representing the current word. Modifying this string has not effect on the trie.
             */
            std::basic_string<T> operator*() {
                return build_word();
            }

        private:
            class ArrowWrapper;
        public:
            /**
             * @return The string inside a wrapper class to allow access to address of temporary. Does not allow
             * modification of trie.
             */
            ArrowWrapper operator->() {
                return ArrowWrapper(build_word());
            }

            bool operator==(const iterator& other) const {
                return this->m_Index == other.m_Index;
            }

            bool operator!=(const iterator& other) const {
                return this->m_Index != other.m_Index;
            }

        private:
            /**
             * @brief Left hand side depth first traversal to find next element in trie.
             */
            void next_word() {
                if (this->m_Index == m_Trie.size()) {
                    // DONE ITERATING
                    return;
                }

                while (true) {
                    T** children = m_NodeStack.top();
                    decltype(m_Trie.m_Alphabet.size()) i
                            = m_SymbolStack.empty() || m_SymbolStack.size() == m_NodeStack.size() - 1
                            ? 0
                            : m_SymbolStack.top() + 1;
                    for (; i < m_Trie.m_Alphabet.size(); ++i) {
                        if (children[i] != nullptr) {
                            break;
                        }
                    }
                    if (m_SymbolStack.size() > m_NodeStack.size() - 1)
                        m_SymbolStack.pop();
                    if (i == m_Trie.m_Alphabet.size()) {
                        if (m_NodeStack.top() == m_Trie.m_Root) {
                            // DONE ITERATING
                            m_Index++;
                            return;
                        } else {
                            // BACKTRACK
                            m_NodeStack.pop();
                        }
                    } else {
                        // GO DEEPER
                        m_SymbolStack.push(i);
                        m_NodeStack.push(reinterpret_cast<T**>(children[i]));
                    }
                    // FOUND NEXT WORD
                    if (m_SymbolStack.size() == m_Trie.m_WordLength && m_NodeStack.size() - 1  == m_SymbolStack.size()) {
                        m_Index++;
                        return;
                    }
                }

            }

            /**
             * @brief Builds a word from the current symbol stack
             * @returns The Word built from the current symbol stack
             */
            std::basic_string<T> build_word() {
                std::stack<int> copy{m_SymbolStack};
                std::basic_string<T> rv{};
                rv.resize(copy.size());
                auto counter = copy.size() - 1;
                while (!copy.empty()) {
                    rv[counter] = m_Trie.m_Alphabet[copy.top()];
                    copy.pop();
                    counter--;
                }
                return rv;
            }

            const basic_static_trie<T>& m_Trie;
            std::stack<T**> m_NodeStack;
            std::stack<int> m_SymbolStack;
            size_t m_Index;
            class ArrowWrapper {
                const std::basic_string<T> value;
            public:
                explicit ArrowWrapper(std::basic_string<T> value) : value(value) {}
                const std::basic_string<T>* operator->() const {
                    return &value;
                }
            };
        };
    public:
        constexpr basic_static_trie(const std::size_t wordLength, const std::basic_string<T>& alphabet) :
                m_Alphabet(alphabet),
                m_WordLength(wordLength),
                m_Size(0) {
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
            m_Root = new T*[m_Alphabet.size()]{nullptr};
        };
        constexpr basic_static_trie(const std::size_t wordLength, const std::basic_string<T>& alphabet, std::initializer_list<std::basic_string<T>> words) :
                m_Alphabet(alphabet),
                m_WordLength(wordLength) {
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
            m_Size = 0;
            m_Root = new T*[m_Alphabet.size()]{nullptr};
            for (const auto& word : words) {
                insert(word);
            }
        };
        constexpr basic_static_trie(const basic_static_trie<T>& other) :
        m_Alphabet(other.m_Alphabet), m_WordLength(other.m_WordLength) {
            m_Size = 0;
            m_Root = new T*[m_Alphabet.size()]{nullptr};
            for (const auto& word : other) {
                insert(word);
            }
        }
        constexpr basic_static_trie(basic_static_trie<T>&& other) noexcept :
        m_Alphabet(other.m_Alphabet), m_WordLength(other.m_WordLength) {
            m_Size = other.m_Size;
            m_Root = other.m_Root;
            other.m_Size = 0;
            other.m_Root = new T*[m_Alphabet.size()]{nullptr};
        }
        constexpr basic_static_trie& operator=(const basic_static_trie& other) {
            if (this == &other)
                return *this;
            recursive_delete(m_Root);
            m_Alphabet = other.m_Alphabet;
            m_WordLength = other.m_WordLength;
            m_Size = 0;
            m_Root = new T*[m_Alphabet.size()]{nullptr};
            for (const auto& word : other) {
                insert(word);
            }
            return *this;
        }
        constexpr basic_static_trie& operator=(basic_static_trie&& other) noexcept {
            if (this == &other)
                return *this;
            recursive_delete(m_Root);
            m_Alphabet = other.m_Alphabet;
            m_WordLength = other.m_WordLength;
            m_Size = other.m_Size;
            m_Root = other.m_Root;
            other.m_Root = new T*[m_Alphabet.size()]{nullptr};
            other.m_Size = 0;
            return *this;
        }
        constexpr ~basic_static_trie() {
            recursive_delete(m_Root);
        }
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
        [[nodiscard]] constexpr std::size_t size() const {
            return m_Size;
        }
        [[nodiscard]] constexpr std::size_t length() const {
            return m_Size;
        }
        constexpr int longest_prefix(const std::basic_string<T>& string) const {
            auto node = m_Root;
            int count = 0;
            for (const auto& c : string) {
                auto index = m_Alphabet.find(c);
                if (index == std::string::npos || node[index] == nullptr)
                    break;
                count++;
                node = reinterpret_cast<T**>(node[index]);
            }
            return count;
        }

        [[nodiscard]] auto alphabet() const {
            return m_Alphabet;
        }

        [[nodiscard]] auto word_length() const {
            return m_WordLength;
        }

        [[nodiscard]] iterator begin() const {
            return iterator{*this, false};
        }

        [[nodiscard]] iterator end() const {
            return iterator{*this, true};
        }
    };

    [[maybe_unused]] typedef basic_trie<char> trie;
    [[maybe_unused]] typedef basic_static_trie<char> static_trie;
    [[maybe_unused]] typedef basic_trie<wchar_t> wtrie;
    [[maybe_unused]] typedef basic_static_trie<wchar_t> wstatic_trie;
    [[maybe_unused]] typedef basic_trie<char8_t> u8trie;
    [[maybe_unused]] typedef basic_static_trie<char8_t> u8static_trie;
    [[maybe_unused]] typedef basic_trie<char16_t> u16trie;
    [[maybe_unused]] typedef basic_static_trie<char16_t> u16static_trie;
    [[maybe_unused]] typedef basic_trie<char32_t> u32trie;
    [[maybe_unused]] typedef basic_static_trie<char32_t> u32static_trie;
}



#endif //PINEPP_TRIE_HPP
