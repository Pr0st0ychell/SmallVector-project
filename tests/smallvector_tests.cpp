#include "SmallVector.h"
#include <gtest/gtest.h>

TEST(SmallVectorTest, DefaultConstructor_Empty) {
  SmallVector<int, 4> vec;
  EXPECT_EQ(vec.size(), 0);
  EXPECT_EQ(vec.capacity(), 4);
  EXPECT_TRUE(vec.empty());
}

TEST(SmallVectorTest, PushBack_StackStorage) {
  SmallVector<int, 4> vec;
  for (int i = 0; i < 4; ++i)
    vec.push_back(i);
  EXPECT_EQ(vec.size(), 4);
  for (int i = 0; i < 4; ++i)
    EXPECT_EQ(vec[i], i);
}

TEST(SmallVectorTest, PushBack_HeapStorage) {
  SmallVector<int, 4> vec;
  for (int i = 0; i < 5; ++i)
    vec.push_back(i);
  EXPECT_GT(vec.capacity(), 4);
  for (int i = 0; i < 5; ++i)
    EXPECT_EQ(vec[i], i);
}

TEST(SmallVectorTest, CopyAndMoveSemantics) {
  SmallVector<int, 4> vec1;
  vec1.push_back(1);
  vec1.push_back(2);

  SmallVector<int, 4> vec2 = vec1;
  EXPECT_EQ(vec2[0], 1);
  EXPECT_EQ(vec2[1], 2);

  SmallVector<int, 4> vec3 = std::move(vec1);
  EXPECT_EQ(vec3[0], 1);
  EXPECT_EQ(vec3[1], 2);
}