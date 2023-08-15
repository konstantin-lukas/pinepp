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
     * @brief Template class for storing strings without duplicates
     * @details A basic_trie is a structure that allows for constant time lookup and insertion of strings
     * (linear in the size of the inserted/searched string without duplicates. Its worst case space complexity is very high however.
     * It has O(n*k) space complexity where n is the amount of internal nodes
     * (one for each unique character inserted in one position) and k is the size of the alphabet.
     * @tparam T The char type to determine the kind of string to use,
     * e.g. char for std::basic_string<char> (which is std::string).
     */
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
        /**
         * @brief Construct an empty trie
         */
        constexpr basic_trie() : m_Size(0), m_Root(std::make_unique<s_Node>()) {};

        /**
         * @brief Construct a trie from a list of \p words
         * @param words
         */
        constexpr basic_trie(std::initializer_list<std::basic_string<T>> words) {
            m_Size = 0;
            m_Root = std::make_unique<s_Node>();
            for (const auto& word : words) {
                insert(word);
            }
        };
        /**
         * @brief Copy constructor
         */
        constexpr basic_trie(const basic_trie<T>& other) {
            m_Size = 0;
            m_Root = std::make_unique<s_Node>();
            for (const auto& word : other) {
                insert(word);
            }
        }
        /**
         * @brief Move constructor
         */
        constexpr basic_trie(basic_trie<T>&& other) noexcept {
            m_Size = other.m_Size;
            m_Root = std::move(other.m_Root);
            other.m_Size = 0;
            other.m_Root = std::make_unique<s_Node>();
        }
        /**
         * @brief Copy assignment operator
         */
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
        /**
         * @brief Move assignment operator
         */
        constexpr basic_trie& operator=(basic_trie&& other) noexcept {
            if (this == &other)
                return *this;
            m_Size = other.m_Size;
            m_Root = std::move(other.m_Root);
            other.m_Size = 0;
            other.m_Root = std::make_unique<s_Node>();
            return *this;
        }
        /**
         * @details Insert a \p string into the trie
         * @param string
         */
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
        /**
         * @details Checks if the trie contains a \p string
         * @param string
         */
        [[nodiscard]] constexpr bool contains(const std::basic_string<T>& string) const {
            auto* node = m_Root.get();
            for (const auto& c : string) {
                if (!node->m_Children.contains(c))
                    return false;
                node = node->m_Children[c].get();
            }
            return node->m_IsFinal;
        }
        /**
         * @returns The amount of unique words in the trie
         */
        [[nodiscard]] constexpr std::size_t size() const {
            return m_Size;
        }
        /**
         * @returns The amount of unique words in the trie
         */
        [[nodiscard]] constexpr std::size_t length() const {
            return m_Size;
        }

        /**
         * @param string
         * @returns The length of the longest prefix you get by traversing the trie
         * along the path of a \p string. For example if your trie only contains the word "Hello", then
         * longest_prefix("Help me") will return 3.
         */
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

        /**
         * @details Remove a \p string from the trie. Complexity: linear in the size of the string.
         * @param string
         */
        void remove(const std::basic_string<T>& string) {
            std::stack<s_Node*> nodes;
            std::stack<T> symbols;
            nodes.push(m_Root.get());
            for (const auto c : string) {
                if (nodes.top()->m_Children.contains(c)) {
                    symbols.push(c);
                    nodes.push(nodes.top()->m_Children[c].get());
                } else {
                    return;
                }
            }
            if (!nodes.top()->m_IsFinal)
                return;
            while (true) {
                if (!nodes.top()->m_Children.empty()) {
                    nodes.top()->m_IsFinal = false;
                    m_Size--;
                    return;
                } else if (nodes.size() > 1) {
                    nodes.pop();
                    nodes.top()->m_Children.erase(symbols.top());
                    symbols.top();
                } else {
                    if (nodes.top()->m_IsFinal){
                        nodes.top()->m_IsFinal = false;
                        m_Size--;
                    }
                    return;
                }
            }
        }

        [[nodiscard]] iterator begin() const {
            return iterator{*this, false};
        }

        [[nodiscard]] iterator end() const {
            return iterator{*this, true};
        }
    };
















    /**
     * @brief Template class for storing strings of fixed length without duplicates using a limited alphabet
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

        /**
         * @details Construct an empty trie with a fixed \p wordLength and a fixed \p alphabet.
         * @param wordLength
         * @param alphabet
         */
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
        /**
         * @details Construct a trie with a fixed \p wordLength and a fixed \p alphabet containing \p words.
         * @param wordLength
         * @param alphabet
         * @param words
         */
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
        /**
         * @brief Copy constructor
         */
        constexpr basic_static_trie(const basic_static_trie<T>& other) :
        m_Alphabet(other.m_Alphabet), m_WordLength(other.m_WordLength) {
            m_Size = 0;
            m_Root = new T*[m_Alphabet.size()]{nullptr};
            for (const auto& word : other) {
                insert(word);
            }
        }
        /**
         * @brief Move constructor
         */
        constexpr basic_static_trie(basic_static_trie<T>&& other) noexcept :
        m_Alphabet(other.m_Alphabet), m_WordLength(other.m_WordLength) {
            m_Size = other.m_Size;
            m_Root = other.m_Root;
            other.m_Size = 0;
            other.m_Root = new T*[m_Alphabet.size()]{nullptr};
        }
        /**
         * @brief Copy assignment operator
         */
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
        /**
         * @brief Move assignment operator
         */
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
        /**
         * @brief Destructor (uses a recursive method)
         */
        constexpr ~basic_static_trie() {
            recursive_delete(m_Root);
        }
        /**
         * @details Insert a \p string into the trie. The strings length and used characters have to
         * correspond with the trie's word length and alphabet. Otherwise an exception is thrown.
         * @param string
         */
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
        /**
         * @details Check if the trie contains a certain \p string
         * @param string
         */
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
        /**
         * @returns The amount of unique words in the trie
         */
        [[nodiscard]] constexpr std::size_t size() const {
            return m_Size;
        }
        /**
         * @returns The amount of unique words in the trie
         */
        [[nodiscard]] constexpr std::size_t length() const {
            return m_Size;
        }
        /**
         * @param string
         * @returns The length of the longest prefix you get by traversing the trie
         * along the path of a \p string. For example if your trie only contains the word "Hello", then
         * longest_prefix("Help me") will return 3.
         */
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

        /**
         * @details Remove a \p string from the trie.
         * @param string
         */
        void remove(const std::basic_string<T>& string) {
            if (string.size() != m_WordLength)
                return;
            std::stack<T**> nodes;
            std::stack<T> symbols;
            nodes.push(m_Root);
            for (const auto c : string) {
                auto index = m_Alphabet.find(c);
                if (nodes.top()[index]) {
                    symbols.push(c);
                    nodes.push(reinterpret_cast<T**>(nodes.top()[index]));
                } else {
                    return;
                }
            }
            while (true) {
                bool hasChildren = false;
                auto top = nodes.top();
                for (decltype(m_Alphabet.size()) i = 0; i < m_Alphabet.size(); ++i) {
                    if (top[i] != nullptr) {
                        hasChildren = true;
                        break;
                    }
                }
                if (hasChildren) {
                    m_Size--;
                    return;
                } else {
                    delete[] nodes.top();
                    nodes.pop();
                    nodes.top()[m_Alphabet.find(symbols.top())] = nullptr;
                    symbols.pop();
                }
            }
        }

        /**
         * @return The current alphabet as a basic_string
         */
        [[nodiscard]] auto alphabet() const {
            return m_Alphabet;
        }

        /**
         * @return The current word length
         */
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
