/**
 * Tests for shape.h
 */
#include <gtest/gtest.h>
#include <numcpp/shape.h>
namespace np = numcpp;

TEST(ShapeTest, Rank1Constructor) {
  np::shape_t<1> shape{10};
  EXPECT_EQ(shape.rank, 1);
  EXPECT_EQ(shape.prod(), 10);
  EXPECT_EQ(shape[0], 10);
}

TEST(ShapeTest, Rank2Constructor) {
  np::shape_t<2> shape{4, 6};
  EXPECT_EQ(shape.rank, 2);
  EXPECT_EQ(shape.prod(), 24);
  EXPECT_EQ(shape[0], 4);
  EXPECT_EQ(shape[1], 6);
}

TEST(ShapeTest, Rank3Constructor) {
  np::shape_t<3> shape{3, 4, 6};
  EXPECT_EQ(shape.rank, 3);
  EXPECT_EQ(shape.prod(), 72);
  EXPECT_EQ(shape[0], 3);
  EXPECT_EQ(shape[1], 4);
  EXPECT_EQ(shape[2], 6);
}

TEST(ShapeTest, Rank1MakeShape) {
  np::shape_t shape = np::make_shape(10);
  EXPECT_EQ(shape.rank, 1);
  EXPECT_EQ(shape.prod(), 10);
  EXPECT_EQ(shape[0], 10);
}

TEST(ShapeTest, Rank2MakeShape) {
  np::shape_t shape = np::make_shape(4, 6);
  EXPECT_EQ(shape.rank, 2);
  EXPECT_EQ(shape.prod(), 24);
  EXPECT_EQ(shape[0], 4);
  EXPECT_EQ(shape[1], 6);
}

TEST(ShapeTest, Rank3MakeShape) {
  np::shape_t shape = np::make_shape(3, 4, 6);
  EXPECT_EQ(shape.rank, 3);
  EXPECT_EQ(shape.prod(), 72);
  EXPECT_EQ(shape[0], 3);
  EXPECT_EQ(shape[1], 4);
  EXPECT_EQ(shape[2], 6);
}

TEST(ShapeTest, IndexingLayoutRight) {
  np::shape_t<2> shape{3, 4};
  np::index_t<2> strides = np::make_strides(shape, np::layout_right);

  ASSERT_EQ(strides[0], 4);
  ASSERT_EQ(strides[1], 1);

  np::index_t<2> index;
  int offset = 0;
  for (index[0] = 0; index[0] < shape[0]; index[0]++) {
    for (index[1] = 0; index[1] < shape[1]; index[1]++) {
      EXPECT_EQ(index[0] * strides[0] + index[1] * strides[1], offset);
      EXPECT_EQ(np::ravel_index(index, shape, np::layout_right), offset);
      EXPECT_EQ(np::unravel_index(offset, shape, np::layout_right), index);
      ++offset;
    }
  }
}

TEST(ShapeTest, IndexingLayoutLeft) {
  np::shape_t<2> shape{3, 4};
  np::index_t<2> strides = np::make_strides(shape, np::layout_left);

  ASSERT_EQ(strides[0], 1);
  ASSERT_EQ(strides[1], 3);

  np::index_t<2> index;
  int offset = 0;
  for (index[1] = 0; index[1] < shape[1]; index[1]++) {
    for (index[0] = 0; index[0] < shape[0]; index[0]++) {
      EXPECT_EQ(index[0] * strides[0] + index[1] * strides[1], offset);
      EXPECT_EQ(np::ravel_index(index, shape, np::layout_left), offset);
      EXPECT_EQ(np::unravel_index(offset, shape, np::layout_left), index);
      ++offset;
    }
  }
}

TEST(ShapeTest, Broadcast) {
  np::shape_t<3> shape1{3, 4, 1};
  np::shape_t<3> shape2{1, 4, 6};
  np::shape_t<3> expected{3, 4, 6};
  EXPECT_EQ(np::broadcast_shapes(shape1, shape2), expected);

  np::shape_t<3> shape3{2, 1, 6};
  EXPECT_THROW(np::broadcast_shapes(shape1, shape3), std::invalid_argument);
}

TEST(ShapeTest, Concat) {
  np::shape_t<1> shape1{1};
  np::shape_t<2> shape2{4, 2};
  np::shape_t<3> shape3{3, 5, 6};
  np::shape_t expected = np::make_shape(1, 4, 2, 3, 5, 6);
  EXPECT_EQ(np::shape_cat(shape1, shape2, shape3), expected);
}
