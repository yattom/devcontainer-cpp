#include <gtest/gtest.h>

#include "hello_world.h"

TEST(HelloWorldTest, Add) {
    EXPECT_EQ(5, HelloWorld::add(2, 3)) << "2 + 3 should equal 5";
}
