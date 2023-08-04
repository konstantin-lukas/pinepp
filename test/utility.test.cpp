//
// Created by konstantin on 04.08.23.
//

#include <string>
#include "gtest/gtest.h"
#include "utility.hpp"

TEST(IsClass, IsTrueIffCalledWithClassType) {
    class X {};
    class Y {};
    typedef char A[5];
    typedef X B[5];
    typedef X C;
    using namespace pine_pp;
    EXPECT_TRUE(is_class<X>::value);
    EXPECT_TRUE(is_class<Y>::value);
    EXPECT_TRUE(is_class<std::string>::value);
    EXPECT_FALSE(is_class<int>::value);
    EXPECT_FALSE(is_class<uint32_t>::value);
    EXPECT_FALSE(is_class<A>::value);
    EXPECT_FALSE(is_class<B>::value);
    EXPECT_TRUE(is_class<C>::value);
}

TEST(Unsigned128BitInt, PrintsTheCorrectNumber) {
    using namespace pine_pp;
    uint128_t i{6};
    testing::internal::CaptureStdout();
    std::cout << i;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "6");
}