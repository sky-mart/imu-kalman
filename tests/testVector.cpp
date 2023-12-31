#include <gtest/gtest.h>
#include <vector.h>

namespace
{

using mart::alloc::Vector;

TEST(VectorTest, default_ctor)
{
    const Vector<int, 2> vec;
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 0);
}

TEST(VectorTest, initlist_ctor)
{
    const Vector<int, 2> vec{5, 21};
    EXPECT_EQ(vec[0], 5);
    EXPECT_EQ(vec[1], 21);
}

TEST(VectorTest, add)
{
    const Vector<int, 2> x{3, 4};
    const Vector<int, 2> y{8, 2};
    const auto z = x + y;
    EXPECT_EQ(z[0], 11);
    EXPECT_EQ(z[1], 6);
}

TEST(VectorTest, assign_add)
{
    Vector<int, 2> x{3, 4};
    const Vector<int, 2> y{8, 2};
    x += y;
    EXPECT_EQ(x[0], 11);
    EXPECT_EQ(x[1], 6);
}

TEST(VectorTest, mul)
{
    const Vector<int, 2> x{3, 4};
    const auto z = x * 5;
    EXPECT_EQ(z[0], 15);
    EXPECT_EQ(z[1], 20);
}

TEST(VectorTest, assign_mul)
{
    Vector<int, 2> x{3, 4};
    x *= 5;
    EXPECT_EQ(x[0], 15);
    EXPECT_EQ(x[1], 20);
}

TEST(VectorTest, subvec)
{
    const Vector<int, 4> x{7, 4, 10, 2};
    const auto y = x.subvec<2>(1);
    EXPECT_EQ(y[0], 4);
    EXPECT_EQ(y[1], 10);
}

TEST(VectorTest, partition)
{
    const Vector<int, 4> x{7, 4, 10, 2};
    const auto y = x.partition<2>();
    EXPECT_EQ(y[0][0], 7);
    EXPECT_EQ(y[0][1], 4);
    EXPECT_EQ(y[1][0], 10);
    EXPECT_EQ(y[1][1], 2);
}

}  // namespace
