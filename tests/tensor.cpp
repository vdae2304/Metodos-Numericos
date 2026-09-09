/**
 * Tests for tensor.h
 */
#include <gtest/gtest.h>
#include <numcpp/tensor.h>
#include <numcpp/random.h>
namespace np = numcpp;

TEST(TensorTest, VectorSizeConstructor) {
  // Constructs an empty vector with 10 elements.
  np::vector<int> vec(10);
  ASSERT_EQ(vec.rank, 1);
  EXPECT_EQ(vec.size(), 10);
  EXPECT_EQ(vec.shape(0), 10);
}

TEST(TensorTest, MatrixSizeConstructor) {
  // Constructs an empty matrix with 3 rows and 4 columns.
  np::matrix<int> mat(3, 4);
  ASSERT_EQ(mat.rank, 2);
  EXPECT_EQ(mat.size(), 12);
  EXPECT_EQ(mat.shape(0), 3);
  EXPECT_EQ(mat.shape(1), 4);
}

TEST(TensorTest, TensorSizeConstructor) {
  // Constructs an empty tensor of shape 2 x 3 x 4.
  np::tensor<int, 3> t(2, 3, 4);
  ASSERT_EQ(t.rank, 3);
  EXPECT_EQ(t.size(), 24);
  EXPECT_EQ(t.shape(0), 2);
  EXPECT_EQ(t.shape(1), 3);
  EXPECT_EQ(t.shape(2), 4);
}

TEST(TensorTest, VectorFillConstructor) {
  // Constructs a vector with 10 elements filled with zeros.
  np::vector<int> vec({10}, 0);
  ASSERT_EQ(vec.rank, 1);
  ASSERT_EQ(vec.size(), 10);
  ASSERT_EQ(vec.shape(0), 10);

  for (int i = 0; i < vec.size(); i++) {
    EXPECT_EQ(vec(i), 0);
  }
}

TEST(TensorTest, MatrixFillConstructor) {
  // Constructs a matrix with 3 rows and 4 columns filled with ones.
  np::matrix<int> mat({3, 4}, 1);
  ASSERT_EQ(mat.rank, 2);
  ASSERT_EQ(mat.size(), 12);
  ASSERT_EQ(mat.shape(0), 3);
  ASSERT_EQ(mat.shape(1), 4);

  for (int i = 0; i < mat.shape(0); i++) {
    for (int j = 0; j < mat.shape(1); j++) {
      EXPECT_EQ(mat(i, i), 1);
    }
  }
}

TEST(TensorTest, TensorFillConstructor) {
  // Constructs a tensor of shape 2 x 3 x 4 filled with 100.
  np::tensor<int, 3> t({2, 3, 4}, 100);
  ASSERT_EQ(t.rank, 3);
  ASSERT_EQ(t.size(), 24);
  ASSERT_EQ(t.shape(0), 2);
  ASSERT_EQ(t.shape(1), 3);
  ASSERT_EQ(t.shape(2), 4);

  for (int i = 0; i < t.shape(0); i++) {
    for (int j = 0; j < t.shape(1); j++) {
      for (int k = 0; k < t.shape(2); k++) {
        EXPECT_EQ(t(i, i, k), 100);
      }
    }
  }
}

TEST(TensorTest, VectorRangeConstructor) {
  // Constructs a vector with its values initialized from a range.
  const int data[10] = {-4, 16, 14, 9, 18, 3, 7, 2, 1, 4};
  np::vector<int> vec(data, 10);

  ASSERT_EQ(vec.rank, 1);
  ASSERT_EQ(vec.size(), 10);
  ASSERT_EQ(vec.shape(0), 10);
  ASSERT_NE(vec.data(), data);

  for (int i = 0; i < vec.size(); i++) {
    EXPECT_EQ(vec(i), data[i]);
  }
}

TEST(TensorTest, MatrixRangeConstructorRight) {
  // Constructs a matrix with its values initialized from a range in right
  // layout.
  const int data[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
  np::matrix<int> mat(data, 3, 4);

  ASSERT_EQ(mat.rank, 2);
  ASSERT_EQ(mat.size(), 12);
  ASSERT_EQ(mat.shape(0), 3);
  ASSERT_EQ(mat.shape(1), 4);
  ASSERT_NE(mat.data(), data);
  ASSERT_EQ(mat.layout(), np::layout_right);

  int offset = 0;
  for (int i = 0; i < mat.shape(0); i++) {
    for (int j = 0; j < mat.shape(1); j++) {
      EXPECT_EQ(mat(i, j), data[offset++]);
    }
  }
}

TEST(TensorTest, MatrixRangeConstructorLeft) {
  // Constructs a matrix with its values initialized from a range in left
  // layout.
  const int data[12] = {0, 10, -4, 5, 6, 10, 8, 12, 2, 11, 0, -1};
  np::matrix<int> mat(data, {3, 4}, np::layout_left);

  ASSERT_EQ(mat.rank, 2);
  ASSERT_EQ(mat.size(), 12);
  ASSERT_EQ(mat.shape(0), 3);
  ASSERT_EQ(mat.shape(1), 4);
  ASSERT_NE(mat.data(), data);
  ASSERT_EQ(mat.layout(), np::layout_left);

  int offset = 0;
  for (int j = 0; j < mat.shape(1); j++) {
    for (int i = 0; i < mat.shape(0); i++) {
      EXPECT_EQ(mat(i, j), data[offset++]);
    }
  }
}

TEST(TensorTest, VectorSlicing) {
  std::default_random_engine urng(42);
  np::vector<int> vec = np::random::integers(0, 100, {10}, urng);

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

TEST(TensorTest, VectorSlicingLast) {
  std::default_random_engine urng(42);
  np::vector<int> vec = np::random::integers(0, 100, {10}, urng);

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

TEST(TensorTest, VectorSlicingStrided) {
  std::default_random_engine urng(42);
  np::vector<int> vec = np::random::integers(0, 100, {10}, urng);

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

TEST(TensorTest, VectorSlicingReversed) {
  std::default_random_engine urng(42);
  np::vector<int> vec = np::random::integers(0, 100, {10}, urng);

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

TEST(TensorTest, MatrixSlicing) {
  std::default_random_engine urng(42);
  np::matrix<int> mat = np::random::integers(0, 100, {3, 4}, urng);

  // Flip matrix horizontally.
  const auto _ = std::nullopt;
  np::matrix_view<int> view = mat(np::slice(), np::slice(_, _, -1));
  ASSERT_EQ(view.rank, 2);
  ASSERT_EQ(view.size(), 12);
  ASSERT_EQ(view.shape(0), 3);
  ASSERT_EQ(view.shape(1), 4);
  ASSERT_EQ(view.strides(0), 4);
  ASSERT_EQ(view.strides(1), -1);
  ASSERT_EQ(view.data() - 3, mat.data());

  for (int i = 0; i < view.shape(0); i++) {
    for (int j = 0; j < view.shape(1); j++) {
      EXPECT_EQ(view(i, j), mat(i, mat.shape(1) - 1 - j));
    }
  }
}

TEST(TensorTest, MatrixSlicingRow) {
  std::default_random_engine urng(42);
  np::matrix<int> mat = np::random::integers(0, 100, {3, 4}, urng);

  // Select first row.
  np::vector_view<int> view = mat(0, np::slice());
  ASSERT_EQ(view.rank, 1);
  ASSERT_EQ(view.size(), 4);
  ASSERT_EQ(view.shape(0), 4);
  ASSERT_EQ(view.strides(0), 1);
  ASSERT_EQ(view.data(), mat.data());

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), mat(0, i));
  }
}

TEST(TensorTest, MatrixSlicingColumn) {
  std::default_random_engine urng(42);
  np::matrix<int> mat = np::random::integers(0, 100, {3, 4}, urng);

  // Select last column.
  np::vector_view<int> view = mat(np::slice(), -1);
  ASSERT_EQ(view.rank, 1);
  ASSERT_EQ(view.size(), 3);
  ASSERT_EQ(view.shape(0), 3);
  ASSERT_EQ(view.strides(0), 4);
  ASSERT_EQ(view.data() - 3, mat.data());

  for (int i = 0; i < view.size(); i++) {
    EXPECT_EQ(view(i), mat(i, mat.shape(1) - 1));
  }
}

TEST(TensorTest, Broadcast) {
  std::default_random_engine urng(42);
  np::matrix<int> mat = np::random::integers(0, 100, {10, 1}, urng);

  // Tile columns.
  np::matrix_view<const int> view = mat.broadcast_to(10, 5);
  ASSERT_EQ(view.rank, 2);
  ASSERT_EQ(view.size(), 50);
  ASSERT_EQ(view.shape(0), 10);
  ASSERT_EQ(view.shape(1), 5);
  ASSERT_EQ(view.data(), mat.data());

  for (int i = 0; i < view.shape(0); i++) {
    for (int j = 0; j < view.shape(1); j++) {
      EXPECT_EQ(view(i, j), mat(i, 0));
    }
  }

  // Invalid shape.
  EXPECT_THROW(mat.broadcast_to(5, 10), std::invalid_argument);
}

TEST(TensorTest, VectorReshapeRowVector) {
  std::default_random_engine urng(42);
  np::vector<int> vec = np::random::integers(0, 100, {10}, urng);

  // Reshape as row vector.
  np::matrix_view<int> view = vec.reshape(1, 10);
  ASSERT_EQ(view.rank, 2);
  ASSERT_EQ(view.size(), 10);
  ASSERT_EQ(view.shape(0), 1);
  ASSERT_EQ(view.shape(1), 10);
  ASSERT_EQ(view.data(), vec.data());

  for (int i = 0; i < vec.size(); i++) {
    EXPECT_EQ(view(0, i), vec(i));
  }
}

TEST(TensorTest, VectorReshapeColumnVector) {
  std::default_random_engine urng(42);
  np::vector<int> vec = np::random::integers(0, 100, {10}, urng);

  // Reshape as column vector.
  np::matrix_view<int> view = vec.reshape(10, 1);
  ASSERT_EQ(view.rank, 2);
  ASSERT_EQ(view.size(), 10);
  ASSERT_EQ(view.shape(0), 10);
  ASSERT_EQ(view.shape(1), 1);
  ASSERT_EQ(view.data(), vec.data());

  for (int i = 0; i < vec.size(); i++) {
    EXPECT_EQ(view(i, 0), vec(i));
  }

  // Invalid shape.
  ASSERT_THROW(vec.reshape(3, 4), std::invalid_argument);
}

TEST(TensorTest, MatrixTranspose) {
  std::default_random_engine urng(42);
  np::matrix<int> mat = np::random::integers(0, 100, {3, 4}, urng);

  // Transpose matrix.
  np::matrix_view<int> view = mat.t();
  ASSERT_EQ(view.size(), 12);
  ASSERT_EQ(view.shape(0), 4);
  ASSERT_EQ(view.shape(1), 3);
  ASSERT_EQ(view.data(), mat.data());

  for (int i = 0; i < mat.shape(0); i++) {
    for (int j = 0; j < mat.shape(1); j++) {
      EXPECT_EQ(view(j, i), mat(i, j));
    }
  }
}

TEST(TensorTest, TensorTranspose) {
  std::default_random_engine urng(42);
  np::tensor<int, 3> t = np::random::integers(0, 100, {2, 3, 4}, urng);

  // Transpose last two axes.
  np::tensor_view<int, 3> view = t.t(0, 2, 1);
  ASSERT_EQ(view.size(), 24);
  ASSERT_EQ(view.shape(0), 2);
  ASSERT_EQ(view.shape(1), 4);
  ASSERT_EQ(view.shape(2), 3);
  ASSERT_EQ(view.data(), t.data());

  for (int i = 0; i < t.shape(0); i++) {
    for (int j = 0; j < t.shape(1); j++) {
      for (int k = 0; k < t.shape(2); k++) {
        EXPECT_EQ(view(i, k, j), t(i, j, k));
      }
    }
  }
}
