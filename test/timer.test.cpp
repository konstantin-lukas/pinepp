//
// Created by konstantin on 06.08.23.
//

#include <gtest/gtest.h>
#include "timer.hpp"

TEST(Timer, StopsTheTime) {
    using namespace pinepp;
    timer t{};

    t.start();
    sleep(1);
    t.pause();
    EXPECT_EQ(t.get_s(), 1);


    t.start();
    sleep(1);
    t.pause();
    EXPECT_EQ(t.get_s(), 2);

    t.reset();
    t.start();
    sleep(1);
    t.pause();
    EXPECT_EQ(t.get_s(), 1);

    t.start();
    auto v1 = t.get_ns();
    auto v2 = t.get_ns();
    EXPECT_LT(v1, v2);
    struct timespec ts{0,100000};
    v1 = t.get_ns();
    nanosleep(&ts, nullptr);
    v2 = t.get_ns();
    EXPECT_GT(v2 - v1, 100000);
}

TEST(Timer, Coverage) {
    pinepp::timer t{};
    EXPECT_NO_THROW(t.start());
    EXPECT_NO_THROW(t.start());
    EXPECT_NO_THROW(t.pause());
    EXPECT_NO_THROW((void)t.get_ns());
    EXPECT_NO_THROW((void)t.get_s());
    EXPECT_NO_THROW(t.start());
    EXPECT_NO_THROW((void)t.get_s());
}