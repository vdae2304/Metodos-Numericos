/**
 * Tests for tensor_view.h
 */
#include <gtest/gtest.h>
#include <numcpp/tensor.h>
namespace np = numcpp;

TEST(TensorView, Vector) {
  int data[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
  np::vector_view<int> vec(data, 10);

  ASSERT_EQ(vec.rank, 1);
  ASSERT_EQ(vec.size(), 10);
  ASSERT_EQ(vec.shape(0), 10);
  ASSERT_EQ(vec.strides(0), 1);
  ASSERT_EQ(vec.data(), data);

  for (int i = 0; i < vec.size(); i++) {
    EXPECT_EQ(vec(i), data[i]);
  }
}

TEST(TensorView, MatrixLayoutRight) {
  int data[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
  np::matrix_view<int> mat(data, 3, 4);

  ASSERT_EQ(mat.rank, 2);
  ASSERT_EQ(mat.size(), 12);
  ASSERT_EQ(mat.shape(0), 3);
  ASSERT_EQ(mat.shape(1), 4);
  ASSERT_EQ(mat.strides(0), 4);
  ASSERT_EQ(mat.strides(1), 1);
  ASSERT_EQ(mat.data(), data);

  int offset = 0;
  for (int i = 0; i < mat.shape(0); i++) {
    for (int j = 0; j < mat.shape(1); j++) {
      EXPECT_EQ(mat(i, j), data[offset++]);
    }
  }
}

TEST(TensorView, MatrixLayoutLeft) {
  int data[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
  np::matrix_view<int> mat(data, {3, 4}, np::layout_left);

  ASSERT_EQ(mat.rank, 2);
  ASSERT_EQ(mat.size(), 12);
  ASSERT_EQ(mat.shape(0), 3);
  ASSERT_EQ(mat.shape(1), 4);
  ASSERT_EQ(mat.strides(0), 1);
  ASSERT_EQ(mat.strides(1), 3);
  ASSERT_EQ(mat.data(), data);

  int offset = 0;
  for (int j = 0; j < mat.shape(1); j++) {
    for (int i = 0; i < mat.shape(0); i++) {
      EXPECT_EQ(mat(i, j), data[offset++]);
    }
  }
}

TEST(TensorView, VectorSlicing) {
  int data[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
  np::vector_view<int> vec(data, 10);

  // Select first 5 elements.
  const auto _ = std::nullopt;
  np::vector_view<int> view = vec(np::slice(5));
  ASSERT_EQ(view.size(), 5);
  ASSERT_EQ(view.shape(0), 5);
  ASSERT_EQ(view.strides(0), 1);
  ASSERT_EQ(view.data(), data);

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), vec(i));
  }

  // Select last 3 elements.
  view = std::move(vec(np::slice(-3, _)));
  ASSERT_EQ(view.size(), 3);
  ASSERT_EQ(view.shape(0), 3);
  ASSERT_EQ(view.strides(0), 1);
  ASSERT_EQ(view.data() - 7, data);

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), vec(vec.size() - 3 + i));
  }

  // Select positions 1, 3, 5, 7.
  view = std::move(vec(np::slice(1, 8, 2)));
  ASSERT_EQ(view.size(), 4);
  ASSERT_EQ(view.shape(0), 4);
  ASSERT_EQ(view.strides(0), 2);
  ASSERT_EQ(view.data() - 1, data);

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), vec(2 * i + 1));
  }

  // Loop in reversed order.
  view = std::move(vec(np::slice(_, _, -1)));
  ASSERT_EQ(view.size(), 10);
  ASSERT_EQ(view.shape(0), 10);
  ASSERT_EQ(view.strides(0), -1);
  ASSERT_EQ(view.data() - 9, data);

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), vec(vec.size() - 1 - i));
  }
}

TEST(TensorView, MatrixSlicing) {
  int data[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
  np::matrix_view<int> mat(data, 3, 4);

  // Select first row.
  const auto _ = std::nullopt;
  np::vector_view<int> view = mat(0, np::slice(_, _));
  ASSERT_EQ(view.rank, 1);
  ASSERT_EQ(view.size(), 4);
  ASSERT_EQ(view.shape(0), 4);
  ASSERT_EQ(view.strides(0), 1);
  ASSERT_EQ(view.data(), data);

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), mat(0, i));
  }

  // Select last column.
  view = std::move(mat(np::slice(_, _), -1));
  ASSERT_EQ(view.rank, 1);
  ASSERT_EQ(view.size(), 3);
  ASSERT_EQ(view.shape(0), 3);
  ASSERT_EQ(view.strides(0), 4);
  ASSERT_EQ(view.data() - 3, data);

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), mat(i, mat.shape(1) - 1));
  }

  // Flip horizontally.
  np::matrix_view<int> view2d = mat(np::slice(_, _),
                                    np::slice(_, _, -1));
  ASSERT_EQ(view2d.rank, 2);
  ASSERT_EQ(view2d.size(), 12);
  ASSERT_EQ(view2d.shape(0), 3);
  ASSERT_EQ(view2d.shape(1), 4);
  ASSERT_EQ(view2d.strides(0), 4);
  ASSERT_EQ(view2d.strides(1), -1);
  ASSERT_EQ(view2d.data() - 3, data);

  for (int i = 0; i < view2d.shape(0); i++) {
    for (int j = 0; j < view2d.shape(1); j++) {
      EXPECT_EQ(view2d(i, j), mat(i, mat.shape(1) - 1 - j));
    }
  }
}

TEST(TensorView, Broadcast) {
  int data[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
  np::matrix_view<int> mat(data, 10, 1);

  np::matrix_view<const int> view = mat.broadcast_to(10, 5);
  ASSERT_EQ(view.rank, 2);
  ASSERT_EQ(view.size(), 50);
  ASSERT_EQ(view.shape(0), 10);
  ASSERT_EQ(view.shape(1), 5);
  ASSERT_EQ(view.data(), data);

  for (int i = 0; i < view.shape(0); i++) {
    for (int j = 0; j < view.shape(1); j++) {
      EXPECT_EQ(view(i, j), mat(i, 0));
    }
  }
}

TEST(TensorView, VectorReshape) {
  int data[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
  np::vector_view<int> vec(data, 10);

  // Reshape as row vector.
  np::matrix_view<int> view = vec.reshape(1, 10);
  ASSERT_EQ(view.rank, 2);
  ASSERT_EQ(view.size(), 10);
  ASSERT_EQ(view.shape(0), 1);
  ASSERT_EQ(view.shape(1), 10);
  ASSERT_EQ(view.data(), data);

  for (int i = 0; i < vec.size(); i++) {
    EXPECT_EQ(vec(i), view(0, i));
  }

  // Reshape as column vector.
  view = std::move(vec.reshape(10, 1));
  ASSERT_EQ(view.rank, 2);
  ASSERT_EQ(view.size(), 10);
  ASSERT_EQ(view.shape(0), 10);
  ASSERT_EQ(view.shape(1), 1);
  ASSERT_EQ(view.data(), data);

  for (int i = 0; i < vec.size(); i++) {
    EXPECT_EQ(vec(i), view(i, 0));
  }

  ASSERT_THROW(vec.reshape(3, 4), std::invalid_argument);
}

TEST(TensorView, MatrixTranspose) {
  int data[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
  np::matrix_view<int> mat(data, 3, 4);

  np::matrix_view<int> view = mat.t();
  ASSERT_EQ(view.size(), 12);
  ASSERT_EQ(view.shape(0), 4);
  ASSERT_EQ(view.shape(1), 3);
  ASSERT_EQ(view.data(), data);

  for (int i = 0; i < mat.shape(0); i++) {
    for (int j = 0; j < mat.shape(1); j++) {
      EXPECT_EQ(view(j, i), mat(i, j));
    }
  }
}
