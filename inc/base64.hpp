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
    std::string base64_encode(const std::string& input);
    std::string base64_encode(const bit_pattern& input, bool padding = true);

}



#endif //PINEPP_BASE64_HPP
