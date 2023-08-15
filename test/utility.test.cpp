//
// Created by konstantin on 04.08.23.
//

#include <string>
#include <regex>
#include <fstream>
#include <sys/resource.h>
#include "gtest/gtest.h"
#include "utility.hpp"

TEST(IsClass, IsTrueIffCalledWithClassType) {
    class X {};
    class Y {};
    typedef char A[5];
    typedef X B[5];
    typedef X C;
    using namespace pinepp;
    EXPECT_TRUE(is_class<X>::value);
    EXPECT_TRUE(is_class<Y>::value);
    EXPECT_TRUE(is_class<std::string>::value);
    EXPECT_FALSE(is_class<int>::value);
    EXPECT_FALSE(is_class<uint32_t>::value);
    EXPECT_FALSE(is_class<A>::value);
    EXPECT_FALSE(is_class<B>::value);
    EXPECT_TRUE(is_class<C>::value);
}

TEST(PrintIterable, PrintsIterableContainersAsCommaSeparatedLists) {
    using namespace pinepp;

    std::vector vec{5,2,53,3,7,658,21};
    testing::internal::CaptureStdout();
    print_iterable(vec);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "[5, 2, 53, 3, 7, 658, 21]");

    vec.emplace_back(363);
    testing::internal::CaptureStdout();
    print_iterable(vec);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "[5, 2, 53, 3, 7, 658, 21, 363]");
}

void fetchTestString(pinepp::http_tools tool) {
    using namespace pinepp;
    const auto reply = fetch("https://www.google.com", tool);
    std::regex regex("^<!DOCTYPE html>", std::regex_constants::icase);
    EXPECT_TRUE(regex_search(reply,regex));
}

void fetchTestFile(pinepp::http_tools tool) {
    using namespace pinepp;
    std::string fp = "/tmp/test.html";
    std::string resource = "https://konstantinlukas.de";
    fetch(resource, fp, tool);
    std::ifstream file{fp};
    if (file.is_open()) {
        std::string contents((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
        EXPECT_EQ(contents, fetch(resource, tool));
    } else {
        FAIL();
    }
}

TEST(Fetch, CanUseCurlToRetrieveAResourceViaHTTPAndReturnItAsAString) {
    fetchTestString(pinepp::http_tools::CURL);
}

TEST(Fetch, CanUseCurlToRetrieveAResourceViaHTTPAndSaveItInAFile) {
    fetchTestFile(pinepp::http_tools::CURL);
}

TEST(Fetch, CanUseWgetToRetrieveAResourceViaHTTPAndReturnItAsAString) {
    fetchTestString(pinepp::http_tools::WGET);
}

TEST(Fetch, CanUseWgetToRetrieveAResourceViaHTTPAndSaveItInAFile) {
    fetchTestFile(pinepp::http_tools::WGET);
}

TEST(Fetch, Coverage) {
    using namespace pinepp;
    EXPECT_THROW(fetch("https://konstantinlukas.de", "/uhghwao/wamk.txt"),std::runtime_error);
    const rlimit lim{4,4};
    setrlimit(RLIMIT_NOFILE, &lim);
    EXPECT_THROW(fetch("https://konstantinlukas.de"),std::runtime_error);

}