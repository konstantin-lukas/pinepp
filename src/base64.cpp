//
// Created by konstantin on 31.05.23.
//

#include <regex>
#include "base64.hpp"

std::string pinepp::base64_encode(const std::string& str) {
    constexpr static char BASE64_TABLE[64] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };

    std::string rv{};
    rv.reserve((size_t)((double)str.size() * 1.25));
    const auto byte_count = str.size();
    auto bytes_in_whole_blocks = 3 * (int)(byte_count / 3);

    for (decltype(bytes_in_whole_blocks) i = 0; i < bytes_in_whole_blocks; i += 3) {
        int i1 = (str[i] >> 2) & 63;
        int i2 = ((str[i] << 4) & 48) | ((str[i + 1] >> 4) & 15);
        int i3 = ((str[i + 1] << 2) & 60) | ((str[i + 2] >> 6) & 3);
        int i4 = str[i + 2] & 63;
        rv.push_back(BASE64_TABLE[i1]);
        rv.push_back(BASE64_TABLE[i2]);
        rv.push_back(BASE64_TABLE[i3]);
        rv.push_back(BASE64_TABLE[i4]);
    }

    auto remaining_chars = byte_count - bytes_in_whole_blocks;
    if (remaining_chars == 1) {
        int i1 = (str[byte_count - 1] >> 2) & 63;
        int i2 = 0 | ((str[byte_count - 1] << 4) & 48);
        rv.push_back(BASE64_TABLE[i1]);
        rv.push_back(BASE64_TABLE[i2]);
        rv.push_back('=');
        rv.push_back('=');
    } else if (remaining_chars == 2) {
        int i1 = (str[byte_count - 2] >> 2) & 63;
        int i2 = ((str[byte_count - 2] << 4) & 48) | ((str[byte_count - 1] >> 4) & 15);
        int i3 = 0 | ((str[byte_count - 1] << 2) & 60);
        rv.push_back(BASE64_TABLE[i1]);
        rv.push_back(BASE64_TABLE[i2]);
        rv.push_back(BASE64_TABLE[i3]);
        rv.push_back('=');
    }

    rv.shrink_to_fit();
    return rv;
}

std::string pinepp::base64_decode(const std::string& base64) {

    std::regex base64_pattern("^[A-Za-z0-9+/]+={0,2}$");
    if (base64.size() % 4 != 0 || !std::regex_match(base64, base64_pattern)) {
        if (base64.empty())
            return std::string{};
        throw std::invalid_argument{"Given string is not a valid base64 encoding"};
    }


    const auto char_count = base64.size();
    int padding_len = 0;
    if (base64[char_count - 1] == '=')
        padding_len++;
    if (base64[char_count - 2] == '=')
        padding_len++;

    std::string rv{};
    rv.reserve((size_t)((double)base64.size() * 0.75));
    uint8_t idx[4];
    for (auto i = decltype(char_count){0}; i < char_count; i += 4) {
        for (int j = 0; j < 4; ++j) {
            if (base64[i + j] >= 'A' && base64[i + j] <= 'Z')
                idx[j] = base64[i + j] - 'A';
            else if (base64[i + j] >= 'a' && base64[i + j] <= 'z')
                idx[j] = base64[i + j] - 'a' + 26;
            else if (base64[i + j] >= '0' && base64[i + j] <= '9')
                idx[j] = base64[i + j] - '0' + 52;
            else if (base64[i + j] == '+')
                idx[j] = 62;
            else if (base64[i + j] == '/')
                idx[j] = 63;
            else
                idx[j] = -1;
        }

        auto c1 = static_cast<char>(((idx[0] << 2) & 252) | ((idx[1] >> 4) & 3));
        rv.push_back(c1);
        if (i != char_count - 4 || (i == char_count - 4 && padding_len <= 1)) {
            auto c2 = static_cast<char>(((idx[1] << 4) & 0xf0) | ((idx[2] >> 2) & 0x0f));
            rv.push_back(c2);
        }
        if (i != char_count - 4 || (i == char_count - 4 && padding_len == 0)) {
            auto c3 = static_cast<char>(((idx[2] << 6) & 192) | (idx[3] & 63));
            rv.push_back(c3);
        }
    }

    rv.shrink_to_fit();
    return rv;
}