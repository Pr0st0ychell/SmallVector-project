#include <gtest/gtest.h>
#include "SmallVector.h"

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
    EXPECT_EQ(vec.capacity(), 4);
    for (int i = 0; i < 4; ++i)
        EXPECT_EQ(vec[i], i);
}

TEST(SmallVectorTest, PushBack_HeapStorage) {
    SmallVector<int, 4> vec;
    for (int i = 0; i < 5; ++i)
        vec.push_back(i);

    EXPECT_EQ(vec.size(), 5);
    EXPECT_GT(vec.capacity(), 4);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(vec[i], i);
}

TEST(SmallVectorTest, CopyConstructor) {
    SmallVector<int, 4> vec1;
    vec1.push_back(10);
    vec1.push_back(20);

    SmallVector<int, 4> vec2(vec1);
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0], 10);
    EXPECT_EQ(vec2[1], 20);
}

TEST(SmallVectorTest, MoveConstructor) {
    SmallVector<int, 4> vec1;
    vec1.push_back(10);
    vec1.push_back(20);

    SmallVector<int, 4> vec2(std::move(vec1));
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0], 10);
    EXPECT_EQ(vec2[1], 20);
    EXPECT_EQ(vec1.size(), 0); // vec1 должен быть валидным, но пустым
}

TEST(SmallVectorTest, Resize_SameOrSmaller) {
    SmallVector<int, 4> vec;
    vec.resize(2);
    EXPECT_EQ(vec.size(), 2);
    for (int i = 0; i < 2; ++i)
        EXPECT_EQ(vec[i], 0);
}