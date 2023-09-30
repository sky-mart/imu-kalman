#include <kalman.h>
#include <gtest/gtest.h>

namespace {

using mart::Vector;
using mart::Matrix;

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

TEST(MatrixTest, default_ctor)
{
    const Matrix<int, 2, 2> mat;
    EXPECT_EQ(mat(0, 0), 0);
    EXPECT_EQ(mat(0, 1), 0);
    EXPECT_EQ(mat(1, 0), 0);
    EXPECT_EQ(mat(1, 1), 0);
}

TEST(MatrixTest, initlist_ctor)
{
    const Matrix<int, 2, 2> mat{5, 21, 8, 40};
    EXPECT_EQ(mat(0, 0), 5);
    EXPECT_EQ(mat(0, 1), 21);
    EXPECT_EQ(mat(1, 0), 8);
    EXPECT_EQ(mat(1, 1), 40);
}

TEST(MatrixTest, add)
{
    const Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    const Matrix<int, 2, 2> Y = {
        10, 1,
        7, 6
    };
    const auto Z = X + Y;
    EXPECT_EQ(Z(0, 0), 15);
    EXPECT_EQ(Z(0, 1), 22);
    EXPECT_EQ(Z(1, 0), 15);
    EXPECT_EQ(Z(1, 1), 46);
}

TEST(MatrixTest, assign_add)
{
    Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    const Matrix<int, 2, 2> Y = {
        10, 1,
        7, 6
    };
    X += Y;
    EXPECT_EQ(X(0, 0), 15);
    EXPECT_EQ(X(0, 1), 22);
    EXPECT_EQ(X(1, 0), 15);
    EXPECT_EQ(X(1, 1), 46);
}

TEST(MatrixTest, multiply_vector)
{
    const Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    const Vector<int, 2> a{3, 4};
    const auto b = X * a;
    EXPECT_EQ(b[0], 99);
    EXPECT_EQ(b[1], 184);
}

TEST(MatrixTest, multiply_matrix)
{
    const Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    const Matrix<int, 2, 2> Y = {
        10, 1,
        7, 6
    };
    const auto Z = X * Y;
    EXPECT_EQ(Z(0, 0), 197);
    EXPECT_EQ(Z(0, 1), 131);
    EXPECT_EQ(Z(1, 0), 360);
    EXPECT_EQ(Z(1, 1), 248);
}

TEST(MatrixTest, transpose)
{
    const Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    const auto Z = X.transpose();
    EXPECT_EQ(Z(0, 0), 5);
    EXPECT_EQ(Z(0, 1), 8);
    EXPECT_EQ(Z(1, 0), 21);
    EXPECT_EQ(Z(1, 1), 40);
}

TEST(MatrixTest, transpose_ref)
{
    const Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    Matrix<int, 2, 2> Z;
    X.transpose(Z);
    EXPECT_EQ(Z(0, 0), 5);
    EXPECT_EQ(Z(0, 1), 8);
    EXPECT_EQ(Z(1, 0), 21);
    EXPECT_EQ(Z(1, 1), 40);
}


}
