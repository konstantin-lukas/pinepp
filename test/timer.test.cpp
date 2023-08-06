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
    EXPECT_LE(v1, v2);
}