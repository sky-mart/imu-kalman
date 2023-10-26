#include <matrix.h>
#include <gtest/gtest.h>

namespace {

using mart::Vector;
using mart::Matrix;

TEST(MatrixTest, default_ctor)
{
    const mart::alloc::Matrix<int, 2, 2> mat;
    EXPECT_EQ(mat(0, 0), 0);
    EXPECT_EQ(mat(0, 1), 0);
    EXPECT_EQ(mat(1, 0), 0);
    EXPECT_EQ(mat(1, 1), 0);
}

TEST(MatrixTest, initlist_ctor)
{
    const mart::alloc::Matrix<int, 2, 2> mat{5, 21, 8, 40};
    EXPECT_EQ(mat(0, 0), 5);
    EXPECT_EQ(mat(0, 1), 21);
    EXPECT_EQ(mat(1, 0), 8);
    EXPECT_EQ(mat(1, 1), 40);
}

TEST(MatrixTest, add)
{
    const mart::alloc::Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    const mart::alloc::Matrix<int, 2, 2> Y = {
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
    mart::alloc::Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    const mart::alloc::Matrix<int, 2, 2> Y = {
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
    const mart::alloc::Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    const mart::alloc::Vector<int, 2> a{3, 4};
    const auto b = X * a;
    EXPECT_EQ(b[0], 99);
    EXPECT_EQ(b[1], 184);
}

TEST(MatrixTest, multiply_matrix)
{
    const mart::alloc::Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    const mart::alloc::Matrix<int, 2, 2> Y = {
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
    const mart::alloc::Matrix<int, 2, 2> X = {
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
    const mart::alloc::Matrix<int, 2, 2> X = {
        5, 21,
        8, 40
    };
    mart::alloc::Matrix<int, 2, 2> Z;
    X.transpose(Z);
    EXPECT_EQ(Z(0, 0), 5);
    EXPECT_EQ(Z(0, 1), 8);
    EXPECT_EQ(Z(1, 0), 21);
    EXPECT_EQ(Z(1, 1), 40);
}

TEST(MatrixTest, lu_decomposition_2x2)
{
    const mart::alloc::Matrix<float, 2, 2> X = {
        5, 21,
        8, 40
    };
    mart::alloc::Matrix<float, 2, 2> L, U;
    X.luDecompose(L, U);

    EXPECT_FLOAT_EQ(L(0, 0), 1);
    EXPECT_FLOAT_EQ(L(0, 1), 0);
    EXPECT_FLOAT_EQ(L(1, 0), 1.6f);
    EXPECT_FLOAT_EQ(L(1, 1), 1);

    EXPECT_FLOAT_EQ(U(0, 0), 5);
    EXPECT_FLOAT_EQ(U(0, 1), 21);
    EXPECT_FLOAT_EQ(U(1, 0), 0);
    EXPECT_FLOAT_EQ(U(1, 1), 40.0f - 1.6f * 21.0f);
}

TEST(MatrixTest, lu_decomposition_3x3)
{
    const mart::alloc::Matrix<float, 3, 3> X = {
        3, 7, 5,
        -4, 8, 1,
        10, 0, 14
    };
    mart::alloc::Matrix<float, 3, 3> L, U;
    X.luDecompose(L, U);

    EXPECT_FLOAT_EQ(L(0, 0), 1);
    EXPECT_FLOAT_EQ(L(0, 1), 0);
    EXPECT_FLOAT_EQ(L(0, 2), 0);
    EXPECT_FLOAT_EQ(L(1, 0), -4.0f/3);
    EXPECT_FLOAT_EQ(L(1, 1), 1);
    EXPECT_FLOAT_EQ(L(1, 2), 0);
    EXPECT_FLOAT_EQ(L(2, 0), 10.0f/3);
    EXPECT_FLOAT_EQ(L(2, 1), -35.0f/26);
    EXPECT_FLOAT_EQ(L(2, 2), 1);

    EXPECT_FLOAT_EQ(U(0, 0), 3);
    EXPECT_FLOAT_EQ(U(0, 1), 7);
    EXPECT_FLOAT_EQ(U(0, 2), 5);
    EXPECT_FLOAT_EQ(U(1, 0), 0);
    EXPECT_FLOAT_EQ(U(1, 1), 52.0f/3);
    EXPECT_FLOAT_EQ(U(1, 2), 23.0f/3);
    EXPECT_FLOAT_EQ(U(2, 0), 0);
    EXPECT_FLOAT_EQ(U(2, 1), 0);
    EXPECT_FLOAT_EQ(U(2, 2), 7.6538461538461515f);
}

TEST(MatrixTest, inverse_2x2) {}

TEST(MatrixTest, inverse_3x3) {}

TEST(MatrixTest, submat)
{
    mart::alloc::Matrix<int, 3, 3> X = {
        3, 7, 5,
        -4, 8, 1,
        10, 0, 14
    };

    auto Y = X.submat<2, 2>(1, 1);
    EXPECT_EQ(Y(0, 0), 8);
    EXPECT_EQ(Y(0, 1), 1);
    EXPECT_EQ(Y(1, 0), 0);
    EXPECT_EQ(Y(1, 1), 14);
}

}
