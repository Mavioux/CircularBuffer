#include <gtest/gtest.h>
#include "circular_buffer.h"

class CircularBufferTest : public ::testing::Test {
protected:
    CircularBuffer* cb;

    void SetUp() override {
        cb = circular_buffer_init(3);
    }

    void TearDown() override {
        circular_buffer_free(cb);
    }
};

TEST_F(CircularBufferTest, InitializationTest) {
    ASSERT_TRUE(cb != nullptr);
    ASSERT_TRUE(circular_buffer_is_empty(cb));
    ASSERT_FALSE(circular_buffer_is_full(cb));
}

TEST_F(CircularBufferTest, AddElementTest) {
    ASSERT_TRUE(circular_buffer_add(cb, 1));
    ASSERT_FALSE(circular_buffer_is_empty(cb));
    ASSERT_FALSE(circular_buffer_is_full(cb));
}

TEST_F(CircularBufferTest, RemoveElementTest) {
    circular_buffer_add(cb, 1);
    int element;
    ASSERT_TRUE(circular_buffer_remove(cb, &element));
    ASSERT_EQ(element, 1);
    ASSERT_TRUE(circular_buffer_is_empty(cb));
}

TEST_F(CircularBufferTest, RemoveNonBlockingElementTest) {
    circular_buffer_add(cb, 1);
    int element;
    ASSERT_TRUE(circular_buffer_remove_nonblock(cb, &element));
    ASSERT_EQ(element, 1);
    ASSERT_TRUE(circular_buffer_is_empty(cb));
}

TEST_F(CircularBufferTest, FullBufferTest) {
    circular_buffer_add(cb, 1);
    circular_buffer_add(cb, 2);
    circular_buffer_add(cb, 3);
    ASSERT_TRUE(circular_buffer_is_full(cb));
    ASSERT_TRUE(circular_buffer_add(cb, 4)); // Should overwrite oldest element
}

TEST_F(CircularBufferTest, EMACalculationTest) {
    circular_buffer_add(cb, 100);
    circular_buffer_add(cb, 200);
    double ema = calculate_ema(cb, 0.2);
    ASSERT_GT(ema, 100.0);
    ASSERT_LT(ema, 200.0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}