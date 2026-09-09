/**
 * Tests for tensor_view.h
 */
#include <gtest/gtest.h>
#include <numcpp/tensor.h>
#include <numcpp/random.h>
namespace np = numcpp;

TEST(TensorView, VectorConstructor) {
  // Constructs a vector view referencing an array.
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

TEST(TensorView, MatrixConstructorRight) {
  // Constructs a matrix view referencing an array in right layout.
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

TEST(TensorView, MatrixConstructorLeft) {
  // Constructs a matrix view referencing an array in left layout.
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
  int data[10];
  np::vector_view<int> vec(data, 10);

  std::default_random_engine urng(42);
  vec = np::random::integers(0, 100, vec.shape(), urng);

  // Select first 5 elements.
  np::vector_view<int> view = vec(np::slice(5));
  ASSERT_EQ(view.size(), 5);
  ASSERT_EQ(view.shape(0), 5);
  ASSERT_EQ(view.strides(0), 1);
  ASSERT_EQ(view.data(), vec.data());

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), vec(i));
  }
}

TEST(TensorView, VectorSlicingLast) {
  int data[10];
  np::vector_view<int> vec(data, 10);

  std::default_random_engine urng(42);
  vec = np::random::integers(0, 100, vec.shape(), urng);

  // Select last 3 elements.
  const auto _ = std::nullopt;
  np::vector_view<int> view = vec(np::slice(-3, _));
  ASSERT_EQ(view.size(), 3);
  ASSERT_EQ(view.shape(0), 3);
  ASSERT_EQ(view.strides(0), 1);
  ASSERT_EQ(view.data() - 7, vec.data());

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), vec(vec.size() - 3 + i));
  }
}

TEST(TensorView, VectorSlicingStrided) {
  int data[10];
  np::vector_view<int> vec(data, 10);

  std::default_random_engine urng(42);
  vec = np::random::integers(0, 100, vec.shape(), urng);

  // Select elements in positions 1, 3, 5, 7.
  np::vector_view<int> view = vec(np::slice(1, 8, 2));
  ASSERT_EQ(view.size(), 4);
  ASSERT_EQ(view.shape(0), 4);
  ASSERT_EQ(view.strides(0), 2);
  ASSERT_EQ(view.data() - 1, vec.data());

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), vec(2 * i + 1));
  }
}

TEST(TensorView, VectorSlicingReversed) {
  int data[10];
  np::vector_view<int> vec(data, 10);

  std::default_random_engine urng(42);
  vec = np::random::integers(0, 100, vec.shape(), urng);

  // Select elements in reversed order.
  const auto _ = std::nullopt;
  np::vector_view<int> view = vec(np::slice(_, _, -1));
  ASSERT_EQ(view.size(), 10);
  ASSERT_EQ(view.shape(0), 10);
  ASSERT_EQ(view.strides(0), -1);
  ASSERT_EQ(view.data() + 1 - vec.size(), vec.data());

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), vec(vec.size() - 1 - i));
  }
}

TEST(TensorView, MatrixSlicing) {
  int data[12];
  np::matrix_view<int> mat(data, 3, 4);

  std::default_random_engine urng(42);
  mat = np::random::integers(0, 100, mat.shape(), urng);

  // Flip matrix horizontally.
  const auto _ = std::nullopt;
  np::matrix_view<int> view = mat(np::slice(), np::slice(_, _, -1));
  ASSERT_EQ(view.rank, 2);
  ASSERT_EQ(view.size(), 12);
  ASSERT_EQ(view.shape(0), 3);
  ASSERT_EQ(view.shape(1), 4);
  ASSERT_EQ(view.strides(0), 4);
  ASSERT_EQ(view.strides(1), -1);
  ASSERT_EQ(view.data() - 3, data);

  for (int i = 0; i < view.shape(0); i++) {
    for (int j = 0; j < view.shape(1); j++) {
      EXPECT_EQ(view(i, j), mat(i, mat.shape(1) - 1 - j));
    }
  }
}

TEST(TensorView, MatrixSlicingRow) {
  int data[12];
  np::matrix_view<int> mat(data, 3, 4);

  std::default_random_engine urng(42);
  mat = np::random::integers(0, 100, mat.shape(), urng);

  // Select first row.
  np::vector_view<int> view = mat(0, np::slice());
  ASSERT_EQ(view.rank, 1);
  ASSERT_EQ(view.size(), 4);
  ASSERT_EQ(view.shape(0), 4);
  ASSERT_EQ(view.strides(0), 1);
  ASSERT_EQ(view.data(), data);

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), mat(0, i));
  }
}

TEST(TensorView, MatrixSlicingColumn) {
  int data[12];
  np::matrix_view<int> mat(data, 3, 4);

  std::default_random_engine urng(42);
  mat = np::random::integers(0, 100, mat.shape(), urng);

  // Select last column.
  np::vector_view<int> view = mat(np::slice(), -1);
  ASSERT_EQ(view.rank, 1);
  ASSERT_EQ(view.size(), 3);
  ASSERT_EQ(view.shape(0), 3);
  ASSERT_EQ(view.strides(0), 4);
  ASSERT_EQ(view.data() - 3, data);

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), mat(i, mat.shape(1) - 1));
  }
}

TEST(TensorView, Broadcast) {
  int data[10];
  np::matrix_view<int> mat(data, 10, 1);

  std::default_random_engine urng(42);
  mat = np::random::integers(0, 100, mat.shape(), urng);

  // Tile columns.
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

  // Invalid shape.
  EXPECT_THROW(mat.broadcast_to(5, 10), std::invalid_argument);
}

TEST(TensorView, VectorReshapeRowVector) {
  int data[10];
  np::vector_view<int> vec(data, 10);

  std::default_random_engine urng(42);
  vec = np::random::integers(0, 100, vec.shape(), urng);

  // Reshape as row vector.
  np::matrix_view<int> view = vec.reshape(1, 10);
  ASSERT_EQ(view.rank, 2);
  ASSERT_EQ(view.size(), 10);
  ASSERT_EQ(view.shape(0), 1);
  ASSERT_EQ(view.shape(1), 10);
  ASSERT_EQ(view.data(), data);

  for (int i = 0; i < vec.size(); i++) {
    EXPECT_EQ(view(0, i), vec(i));
  }
}

TEST(TensorView, VectorReshapeColumnVector) {
  int data[10];
  np::vector_view<int> vec(data, 10);

  std::default_random_engine urng(42);
  vec = np::random::integers(0, 100, vec.shape(), urng);

  // Reshape as column vector.
  np::matrix_view<int> view = vec.reshape(10, 1);
  ASSERT_EQ(view.rank, 2);
  ASSERT_EQ(view.size(), 10);
  ASSERT_EQ(view.shape(0), 10);
  ASSERT_EQ(view.shape(1), 1);
  ASSERT_EQ(view.data(), data);

  for (int i = 0; i < vec.size(); i++) {
    EXPECT_EQ(view(i, 0), vec(i));
  }

  // Invalid shape.
  ASSERT_THROW(vec.reshape(3, 4), std::invalid_argument);
}

TEST(TensorView, MatrixTranspose) {
  int data[12];
  np::matrix_view<int> mat(data, 3, 4);

  std::default_random_engine urng(42);
  mat = np::random::integers(0, 100, mat.shape(), urng);

  // Transpose matrix.
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

TEST(TensorView, TensorTranspose) {
  int data[24];
  np::tensor_view<int, 3> t(data, 2, 3, 4);

  std::default_random_engine urng(42);
  t = np::random::integers(0, 100, t.shape(), urng);

  // Transpose last two axes.
  np::tensor_view<int, 3> view = t.t(0, 2, 1);
  ASSERT_EQ(view.size(), 24);
  ASSERT_EQ(view.shape(0), 2);
  ASSERT_EQ(view.shape(1), 4);
  ASSERT_EQ(view.shape(2), 3);
  ASSERT_EQ(view.data(), data);

  for (int i = 0; i < t.shape(0); i++) {
    for (int j = 0; j < t.shape(1); j++) {
      for (int k = 0; k < t.shape(2); k++) {
        EXPECT_EQ(view(i, k, j), t(i, j, k));
      }
    }
  }
}
