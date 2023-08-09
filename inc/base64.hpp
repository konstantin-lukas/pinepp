//
// Created by konstantin on 31.05.23.
//

#ifndef PINEPP_BASE64_HPP
#define PINEPP_BASE64_HPP
#include <string>
#include "bit_pattern.hpp"

namespace pinepp {
    constexpr static char BASE64_TABLE[64] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };

    /**
     * @details Converts an ASCII string to base64.
     * @param str The string to encode
     * @returns A string containing the base64 representation of \p str
     */
    std::string base64_encode(const std::string& str);

    /**
     * @details Converts a base64 encoding to an ASCII string. Performs validity check on base64 string.
     * @param input The base64 string to decode
     * @returns An ASCII string
     */
    std::string base64_decode(const std::string& input);

}



#endif //PINEPP_BASE64_HPP
