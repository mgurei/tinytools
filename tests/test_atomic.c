/**
 * @file test_atomic.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_atomic.h"
#include "tt_types.h"
#include "unity.h"

tt_atomic_int_t atomic;

void setUp() { tt_atomic_init(&atomic, 0); }

void tearDown() { /* Nothing to clean up */ }

void test_atomic_init(void) {
  tt_atomic_int_t test_atomic;
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_atomic_init(&test_atomic, 42));
  TEST_ASSERT_EQUAL(42, tt_atomic_load(&test_atomic, TT_MEMORY_ORDER_RELAXED));
}

void test_atomic_store_load(void) {
  tt_atomic_store(&atomic, 123, TT_MEMORY_ORDER_RELAXED);
  TEST_ASSERT_EQUAL(123, tt_atomic_load(&atomic, TT_MEMORY_ORDER_RELAXED));
}

void test_atomic_add(void) {
  tt_atomic_store(&atomic, 100, TT_MEMORY_ORDER_RELAXED);
  int32_t prev = tt_atomic_add(&atomic, 50, TT_MEMORY_ORDER_RELAXED);
  TEST_ASSERT_EQUAL(100, prev);
  TEST_ASSERT_EQUAL(150, tt_atomic_load(&atomic, TT_MEMORY_ORDER_RELAXED));
}

void test_atomic_sub(void) {
  tt_atomic_store(&atomic, 100, TT_MEMORY_ORDER_RELAXED);
  int32_t prev = tt_atomic_sub(&atomic, 70, TT_MEMORY_ORDER_RELAXED);
  TEST_ASSERT_EQUAL(100, prev);
  TEST_ASSERT_EQUAL(30, tt_atomic_load(&atomic, TT_MEMORY_ORDER_RELAXED));
}

void test_atomic_compare_exchange_success(void) {
  tt_atomic_store(&atomic, 100, TT_MEMORY_ORDER_RELAXED);
  int32_t expected = 100;
  bool success = tt_atomic_compare_exchange(&atomic, &expected, 200,
                                            TT_MEMORY_ORDER_RELAXED);
  TEST_ASSERT_TRUE(success);
  TEST_ASSERT_EQUAL(200, tt_atomic_load(&atomic, TT_MEMORY_ORDER_RELAXED));
}

void test_atomic_compare_exchange_failure(void) {
  tt_atomic_store(&atomic, 100, TT_MEMORY_ORDER_RELAXED);
  int32_t expected = 999;
  bool success = tt_atomic_compare_exchange(&atomic, &expected, 200,
                                            TT_MEMORY_ORDER_RELAXED);
  TEST_ASSERT_FALSE(success);
  TEST_ASSERT_EQUAL(100, tt_atomic_load(&atomic, TT_MEMORY_ORDER_RELAXED));
  TEST_ASSERT_EQUAL(100, expected);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_atomic_init);
  RUN_TEST(test_atomic_store_load);
  RUN_TEST(test_atomic_add);
  RUN_TEST(test_atomic_sub);
  RUN_TEST(test_atomic_compare_exchange_success);
  RUN_TEST(test_atomic_compare_exchange_failure);
  return UNITY_END();
}
