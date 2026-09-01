/**
 * Tests for tensor.h
 */
#include <gtest/gtest.h>
#include <numcpp/tensor.h>
namespace np = numcpp;

TEST(TensorTest, Vector) {
  int data[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
  np::vector<int> vec{-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};

  ASSERT_EQ(vec.rank, 1);
  ASSERT_EQ(vec.size(), 10);
  ASSERT_EQ(vec.shape(0), 10);

  for (int i = 0; i < vec.size(); i++) {
    EXPECT_EQ(vec(i), data[i]);
  }
}

TEST(TensorTest, Matrix) {
  int data[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
  np::matrix<int> mat{{0, 10, -4, 5}, {6, 10, 8, 12}, {2, 11, 0, -1}};

  ASSERT_EQ(mat.rank, 2);
  ASSERT_EQ(mat.size(), 12);
  ASSERT_EQ(mat.shape(0), 3);
  ASSERT_EQ(mat.shape(1), 4);

  int offset = 0;
  for (int i = 0; i < mat.shape(0); i++) {
    for (int j = 0; j < mat.shape(1); j++) {
      EXPECT_EQ(mat(i, j), data[offset++]);
    }
  }
}
