//
// Created by konstantin on 31.05.23.
//

#ifndef PINEPP_BASE64_HPP
#define PINEPP_BASE64_HPP
#include <string>
#include "bit_pattern.hpp"

namespace pinepp {

    /**
     * @details Converts an ASCII string (std::string) to base64.
     * @param str The string to encode
     * @returns A string containing the base64 representation of \p str
     */
     std::string base64_encode(const std::string& str);

    /**
     * @details Converts a base64 encoding to an ASCII string (std::string). Performs validity check on base64 string.
     * @param input The base64 string to decode
     * @returns An ASCII string
     */
     std::string base64_decode(const std::string& input);

}



#endif //PINEPP_BASE64_HPP
