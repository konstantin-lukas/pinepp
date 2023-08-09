//
// Created by konstantin on 09.08.23.
//

#include "gtest/gtest.h"
#include "bit_pattern.hpp"
#include "base64.hpp"
TEST(Base64EncodeFunction, CanCreateBase64FromAString) {
    using namespace pinepp;

    // NO PADDING
    EXPECT_EQ("SGVsbG8sIHdvcmxkIQpXaGF0J3MgdXA/", base64_encode("Hello, world!\nWhat's up?"));

    // SINGLE PADDING
    EXPECT_EQ("VGhpcyBpcyBhIHRlc3Q=", base64_encode("This is a test"));

    // DOUBLE PADDING
    EXPECT_EQ("SGVsbG8sIHdvcmxkIQ==", base64_encode("Hello, world!"));
}

TEST(Base64EncodeFunction, CanCreateAnAsciiStringFromBase64) {
    using namespace pinepp;

    // NO PADDING
    EXPECT_EQ("Hello, world!\nWhat's up?", base64_decode("SGVsbG8sIHdvcmxkIQpXaGF0J3MgdXA/"));

    // SINGLE PADDING
    EXPECT_EQ("This is a test", base64_decode("VGhpcyBpcyBhIHRlc3Q="));

    // DOUBLE PADDING
    EXPECT_EQ("Hello, world!", base64_decode("SGVsbG8sIHdvcmxkIQ=="));
}