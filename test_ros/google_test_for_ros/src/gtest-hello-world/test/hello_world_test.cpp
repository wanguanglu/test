#include <gtest/gtest.h>
#include <climits>

// bad function:
// for example: how to deal with overflow?
int add(int a, int b){
    return a + b;
}

TEST(NumberCmpTest, ShouldPass){
    ASSERT_EQ(3, add(1,2));
}

TEST(NumberCmpTest, ShouldFail){
    ASSERT_EQ(INT_MAX, add(INT_MAX, 1));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
