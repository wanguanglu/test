/**
 * @file   hello-test.cc
 * @author wanguanglu(wanguanglu@qq.com)
 * @date   2016/12/11 16:57:37
 * @brief
 *
 **/

#include "lib/hello-greet.h"
#include "gtest/gtest.h"

TEST(FactorialTest, Negative) { EXPECT_EQ(get_greet("Bazel"), "Hello Bazel"); }
