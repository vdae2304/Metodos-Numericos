/**
 * Tests for shape.h
 */
#include <gtest/gtest.h>
#include <numcpp/shape.h>

#include <sstream>
namespace np = numcpp;

TEST(ShapeTest, Rank1IO) {
  std::istringstream in("10");
  np::shape_t<1> shape;
  in >> shape;

  EXPECT_EQ(shape.rank, 1);
  EXPECT_EQ(shape.prod(), 10);
  EXPECT_EQ(shape[0], 10);

  std::ostringstream out;
  out << shape;
  EXPECT_EQ(out.str(), "(10,)");
}

TEST(ShapeTest, Rank2IO) {
  std::istringstream in("(4,6)");
  np::shape_t<2> shape;
  in >> shape;

  EXPECT_EQ(shape.rank, 2);
  EXPECT_EQ(shape.prod(), 24);
  EXPECT_EQ(shape[0], 4);
  EXPECT_EQ(shape[1], 6);

  std::ostringstream out;
  out << shape;
  EXPECT_EQ(out.str(), "(4, 6)");
}

TEST(ShapeTest, Rank3IO) {
  std::istringstream in("(3,4,6)");
  np::shape_t<3> shape;
  in >> shape;

  EXPECT_EQ(shape.rank, 3);
  EXPECT_EQ(shape.prod(), 72);
  EXPECT_EQ(shape[0], 3);
  EXPECT_EQ(shape[1], 4);
  EXPECT_EQ(shape[2], 6);

  std::ostringstream out;
  out << shape;
  EXPECT_EQ(out.str(), "(3, 4, 6)");
}

TEST(ShapeTest, IndexingLayoutRight) {
  np::shape_t<2> shape{3, 4};
  ASSERT_EQ(shape[0], 3);
  ASSERT_EQ(shape[1], 4);

  np::index_t<2> strides = np::make_strides(shape, np::layout_right);
  ASSERT_EQ(strides[0], 4);
  ASSERT_EQ(strides[1], 1);

  int offset = 0;
  for (int i = 0; i < shape[0]; i++) {
    for (int j = 0; j < shape[1]; j++) {
      EXPECT_EQ(i * strides[0] + j * strides[1], offset);
      EXPECT_EQ(np::ravel_index({i, j}, shape, np::layout_right), offset);
      EXPECT_EQ(np::unravel_index(offset, shape, np::layout_right),
                np::make_index(i, j));
      ++offset;
    }
  }
}

TEST(ShapeTest, IndexingLayoutLeft) {
  np::shape_t<2> shape{3, 4};
  ASSERT_EQ(shape[0], 3);
  ASSERT_EQ(shape[1], 4);

  np::index_t<2> strides = np::make_strides(shape, np::layout_left);
  ASSERT_EQ(strides[0], 1);
  ASSERT_EQ(strides[1], 3);

  int offset = 0;
  for (int j = 0; j < shape[1]; j++) {
    for (int i = 0; i < shape[0]; i++) {
      EXPECT_EQ(i * strides[0] + j * strides[1], offset);
      EXPECT_EQ(np::ravel_index({i, j}, shape, np::layout_left), offset);
      EXPECT_EQ(np::unravel_index(offset, shape, np::layout_left),
                np::make_index(i, j));
      ++offset;
    }
  }
}

TEST(ShapeTest, Broadcast) {
  np::shape_t<3> shape1{3, 4, 1};
  np::shape_t<3> shape2{1, 4, 6};
  EXPECT_EQ(np::broadcast_shapes(shape1, shape2), np::make_shape(3, 4, 6));

  np::shape_t<3> shape3{2, 1, 6};
  EXPECT_THROW(np::broadcast_shapes(shape1, shape3), std::invalid_argument);
}

TEST(ShapeTest, Concat) {
  np::shape_t<1> shape1{1};
  np::shape_t<2> shape2{4, 2};
  np::shape_t<3> shape3{3, 5, 6};
  EXPECT_EQ(np::shape_cat(shape1, shape2, shape3),
            np::make_shape(1, 4, 2, 3, 5, 6));
}
