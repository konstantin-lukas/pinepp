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

TEST(Base64, Coverage) {
    using namespace pinepp;
    std::string strings[] = {"8gh48gh<w4hwoko!!!!_:_,-.,...,-.,-303962++<|||||326##++623632kg"
                             "esjhsjaqOIWHGHHAGUHAWDHufiwiugfwaGUFGWAUGFWAhgkwhagwhgwhaigwa++",
                             "8gh48gh<w4hwoko!!!!_:_,-.,...,-.,-303962++<|||||326##++623632kg"
                             "esjhsjaqOIWHGHHAGUHAWDHufiwiugfwaGUFGWAUGFWAhgkwhagwhgwhaigwa+",
                             "8gh48gh<w4hwoko!!!!_:_,-.,...,-.,-303962++<|||||326##++623632kg"
                             "esjhsjaqOIWHGHHAGUHAWDHufiwiugfwaGUFGWAUGFWAhgkwhagwhgwhaigwa",
                             "8gh48gh<w4hwoko!!!!_:_,-.,...,-.,-303962++<|||||326##++623632kg"
                             "esjhsjaqOIWHGHHAGUHAWDHufiwiugfwaGUFGWAUGFWAhgkwhagwhgwhaigw",
                             "8gjh3j9pfj0jqfh3q",
                             "8gjh3j9pfj0jqfh3",
                             "8gjh3j9pfj0jqfh",
                             "8gjh3j9pfj0jq",
                             "    ",
                             "   ",
                             "  ",
                             " ",
                             "",
                             "fwuA",
                             "fwu",
                             "fw",
                             "f",
                             "///",
                             "+/+",
                             "_^#,:-><()[]{}\n\t\r&%$§$?´`"};
    for (const auto & str : strings) {
        EXPECT_EQ(str, base64_decode(base64_encode(str)));
    }
    EXPECT_EQ("Base64 encoding with + symboo",
              base64_decode("QmFzZTY0IGVuY29kaW5nIHdpdGggKyBzeW1ib2+="));
    EXPECT_EQ("QmFzZTY0IGVuY29kaW5nIHdpdGggKyBzeW1ib28=",
              base64_encode("Base64 encoding with + symboo"));
    EXPECT_ANY_THROW(base64_decode(";;;;"));

}