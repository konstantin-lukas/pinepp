//
// Created by konstantin on 05.08.23.
//
#include <regex>
#include "bit_pattern.hpp"
#include "utility.hpp"
#include "gtest/gtest.h"
TEST(BitPatternDefaultConstructor, CreatesAnEmptyPattern) {
    using namespace pinepp;
    std::string output;

    bit_pattern bp{};
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");
}

TEST(BitPatternStringConstructor, RecognizesBinaryStringAndConstructsTheObjectFromIt) {
    using namespace pinepp;
    std::string output;

    bit_pattern bp{"10011"};
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "10011");

    bp = bit_pattern{"00011110110"};
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "00011110110");
}
TEST(BitPatternStringConstructor, RecognizesHexStringAndConstructsTheObjectFromIt) {
    using namespace pinepp;
    std::string output;

    bit_pattern bp{"0xFf2"};
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "111111110010");

    bp = bit_pattern{"0Xff04A"};
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "11111111000001001010");
}

TEST(BitPatternCopyConstructor, PerformsADeepCopy) {
    using namespace pinepp;
    bit_pattern bp1{"0x8000"};
    bit_pattern bp2{bp1};
    bp1 = bit_pattern{"0"};
    testing::internal::CaptureStdout();
    std::cout << bp2 << '+' << bp1;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1000000000000000+0");
}

TEST(BitPatternCopyConstructor, MovesDataToNewBitPattern) {
    using namespace pinepp;
    bit_pattern tmp{"0x8000"};
    bit_pattern bp1{std::move(tmp)};
    testing::internal::CaptureStdout();
    std::cout << bp1;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1000000000000000");
}

TEST(BitPatternSizeFunction, ReturnsTheExactAmountOfBitsInPattern) {
    using namespace pinepp;
    bit_pattern bp{"0x8000"};
    EXPECT_EQ(bp.size(), 16);

    bp = bit_pattern{"011"};
    EXPECT_EQ(bp.size(), 3);

    bp = bit_pattern{"0x0"};
    EXPECT_EQ(bp.size(), 4);

    bp = bit_pattern{"0"};
    EXPECT_EQ(bp.size(), 1);

    bp = bit_pattern{};
    EXPECT_EQ(bp.size(), 0);
}

TEST(BitPatternMoveAssignmentOperator, MovesDataToNewBitPattern) {
    using namespace pinepp;
    std::string output;

    bit_pattern bp{"0x8001"};
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1000000000000001");

    bp = bit_pattern{"0xFF"};
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "11111111");
}

TEST(BitPatternMoveAssignmentOperator, LeavesTheOldPatternInAnUndefinedButValidState) {
    using namespace pinepp;
    std::string output;

    bit_pattern bp1{"0x80"};
    bit_pattern bp2{"0x40"};
    testing::internal::CaptureStdout();
    std::cout << bp1;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "10000000");
    testing::internal::CaptureStdout();
    std::cout << bp2;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "01000000");

    bp1 = std::move(bp2);
    testing::internal::CaptureStdout();
    std::cout << bp1;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "01000000");

    bp2 = bit_pattern{"111010"};
    testing::internal::CaptureStdout();
    std::cout << bp2;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "111010");
}

TEST(BitPatternOutputOperator, ProducesTheSameResultsAsThePrintIterableFunctionInReverse) {
    using namespace pinepp;
    std::string o1, o2;
    bit_pattern bp{"100101010111"};
    testing::internal::CaptureStdout();
    std::cout << bp;
    o1 = testing::internal::GetCapturedStdout();
    testing::internal::CaptureStdout();
    print_iterable(bp);
    o2 = testing::internal::GetCapturedStdout();
    o2 = std::regex_replace(o2, std::regex(R"([\D])"), "");
    std::reverse(o2.begin(), o2.end());
    EXPECT_EQ(o1, o2);

    bp = bit_pattern{};
    testing::internal::CaptureStdout();
    std::cout << bp;
    o1 = testing::internal::GetCapturedStdout();
    EXPECT_EQ(o1, "");
}

TEST(SetBitFunction, SetsABitWhereIndexZeroIsTheRightmostBitWhenPrintingToCout) {
    using namespace pinepp;
    bit_pattern bp{"100101010111"};
    testing::internal::CaptureStdout();
    std::cout << bp;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "100101010111");

    bp.set_bit(0, false);
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "100101010110");

    bp.set_bit(3, true);
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "100101011110");

    bp.set_bit(0, false);
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "100101011110");

    bp.set_bit(10, true);
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "110101011110");

    bp.set_bit(10, false);
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "100101011110");

}

TEST(BitPatternReverseFunction, ReversesThePattern) {
    using namespace pinepp;
    bit_pattern bp{"100101010111"};
    bp.reverse();
    testing::internal::CaptureStdout();
    std::cout << bp;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "111010101001");

    bp = bit_pattern{"110"};
    bp.reverse();
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "011");
}

TEST(BitPatternBracketsOperator, AllowsReadOnlyAccessToBits) {
    using namespace pinepp;
    bit_pattern bp{"100101010111"};
    EXPECT_EQ(1, bp[0]);
    EXPECT_EQ(1, bp[1]);
    EXPECT_EQ(1, bp[2]);
    EXPECT_EQ(0, bp[3]);
    EXPECT_EQ(1, bp[4]);
    EXPECT_EQ(0, bp[5]);
    EXPECT_EQ(1, bp[6]);
    EXPECT_EQ(0, bp[7]);
    EXPECT_EQ(1, bp[8]);
    EXPECT_EQ(0, bp[9]);
    EXPECT_EQ(0, bp[10]);
    EXPECT_EQ(1, bp[11]);
}

TEST(BitPatternAmpersandOperator, ShouldPerformALogicalAndOnTheTwoValuesAndReturnTheResult) {
    using namespace pinepp;
    bit_pattern bp1{"100101010111"};
    bit_pattern bp2{"10001010"};
    testing::internal::CaptureStdout();
    std::cout << (bp1 & bp2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "00000010");

    bp2 = bit_pattern{"1111"};
    testing::internal::CaptureStdout();
    std::cout << (bp1 & bp2);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "0111");

    bp1 = bit_pattern{"1010"};
    testing::internal::CaptureStdout();
    std::cout << (bp1 & bp2);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1010");
}

TEST(BitPatternPipeOperator, ShouldPerformALogicalOrOnTheTwoValuesAndReturnTheResult) {
    using namespace pinepp;
    bit_pattern bp1{"100101010111"};
    bit_pattern bp2{"10001010"};
    testing::internal::CaptureStdout();
    std::cout << (bp1 | bp2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "11011111");

    bp2 = bit_pattern{"1111"};
    testing::internal::CaptureStdout();
    std::cout << (bp1 | bp2);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1111");

    bp1 = bit_pattern{"1010"};
    testing::internal::CaptureStdout();
    std::cout << (bp1 | bp2);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1111");
}

TEST(BitPatternCircumflexOperator, ShouldPerformALogicalXorOnTheTwoValuesAndReturnTheResult) {
    using namespace pinepp;
    bit_pattern bp1{"100101010111"};
    bit_pattern bp2{"10001010"};
    testing::internal::CaptureStdout();
    std::cout << (bp1 ^ bp2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "11011101");

    bp2 = bit_pattern{"1111"};
    testing::internal::CaptureStdout();
    std::cout << (bp1 ^ bp2);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1000");

    bp1 = bit_pattern{"1010"};
    testing::internal::CaptureStdout();
    std::cout << (bp1 ^ bp2);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "0101");
}

TEST(BitPatternTildeOperator, ShouldPerformALogicalNegationAndReturnTheResult) {
    using namespace pinepp;
    bit_pattern bp{"100101010111"};
    testing::internal::CaptureStdout();
    std::cout << ~bp;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "011010101000");

    bp = bit_pattern{"1111"};
    testing::internal::CaptureStdout();
    std::cout << ~bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "0000");

    bp = bit_pattern{"1010"};
    testing::internal::CaptureStdout();
    std::cout << ~bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "0101");
}

TEST(BitPatternLeftShiftOperator, ShiftsTheBitsInThePatternLeftByNCharactersAndAttachesOverflowingBitsOnTheRight) {
    using namespace pinepp;
    bit_pattern bp{"1011101010010100"};
    testing::internal::CaptureStdout();
    std::cout << (bp << 3);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1101010010100101");
}

TEST(BitPatternLeftShiftOperator, CanShiftDistancesLongerThanOneByte) {
    using namespace pinepp;
    bit_pattern bp{"1011101010010100"};
    testing::internal::CaptureStdout();
    std::cout << (bp << 10);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "0101001011101010");

    testing::internal::CaptureStdout();
    std::cout << (bp << 20);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1010100101001011");
}

TEST(BitPatternLeftShiftOperator, CanHandlePatternsWhoseLengthIsntAMultipleOfEight) {
    using namespace pinepp;
    bit_pattern bp{"1001011101010010100"};
    testing::internal::CaptureStdout();
    std::cout << (bp << 10);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "0100101001001011101");

    testing::internal::CaptureStdout();
    std::cout << (bp << 20);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "0010111010100101001");

    bp = bit_pattern{"10010"};
    testing::internal::CaptureStdout();
    std::cout << (bp << 3);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "10100");
}


TEST(BitPatternRightShiftOperator, ShiftsTheBitsInThePatternRightByNCharactersAndAttachesOverflowingBitsOnTheLeft) {
    using namespace pinepp;
    bit_pattern bp{"1011101010010100"};
    testing::internal::CaptureStdout();
    std::cout << (bp >> 3);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1001011101010010");
}

TEST(BitPatternRightShiftOperator, CanShiftDistancesLongerThanOneByte) {
    using namespace pinepp;
    bit_pattern bp{"1011101010010100"};
    testing::internal::CaptureStdout();
    std::cout << (bp >> 10);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1010010100101110");

    testing::internal::CaptureStdout();
    std::cout << (bp >> 20);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "0100101110101001");
}

TEST(BitPatternRightShiftOperator, CanHandlePatternsWhoseLengthIsntAMultipleOfEight) {
    using namespace pinepp;
    bit_pattern bp{"1001011101010010100"};
    testing::internal::CaptureStdout();
    std::cout << (bp >> 10);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1010010100100101110");

    testing::internal::CaptureStdout();
    std::cout << (bp >> 20);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "0100101110101001010");

    bp = bit_pattern{"10010"};
    testing::internal::CaptureStdout();
    std::cout << (bp >> 3);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "01010");
}

TEST(BitPatternPlusOperator, ConcatenatesToBitPatterns) {
    using namespace pinepp;
    bit_pattern bp1{"1001011101010010100"};
    bit_pattern bp2{"10100"};
    testing::internal::CaptureStdout();
    std::cout << (bp1 + bp2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "100101110101001010010100");

    bp2 = bit_pattern{};
    testing::internal::CaptureStdout();
    std::cout << (bp1 + bp2);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1001011101010010100");
}

TEST(BitPatternCopyAssignmentOperator, PerformsADeepCopy) {
    using namespace pinepp;
    bit_pattern bp1{"1001011101010010100"};
    bit_pattern bp2{};
    bp2 = bp1;
    bp1.set_bit(0, true);
    testing::internal::CaptureStdout();
    std::cout << bp2;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1001011101010010100");
}

TEST(BitPatternStringAssignmentOperator, OverwritesTheCurrentPatternWithThePatternRepresentedByString) {
    using namespace pinepp;
    bit_pattern bp{};
    testing::internal::CaptureStdout();
    std::cout << bp;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");

    bp = "01";
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "01");

    bp = "0xff1";
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "111111110001");

    bp = "000000000000000010101111111111111111111111111111000000000000000101010010101000000101010101";
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "000000000000000010101111111111111111111111111111000000000000000101010010101000000101010101");
}

TEST(BitPatternResizeFunction, DoesntDoAnythingIfTheNewSizeIsTheSameAsTheOldOne) {
    using namespace pinepp;
    bit_pattern bp{"0111011101"};
    bp.resize(7);
    testing::internal::CaptureStdout();
    std::cout << bp;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1011101");

    bp.resize(12);
    testing::internal::CaptureStdout();
    std::cout << bp;
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "000001011101");
}