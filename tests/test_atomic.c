/**
 * @file test_atomic.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_atomic.h"
#include "tt_test.h"
#include "tt_types.h"

tt_atomic_int_t atomic;

void setUp() { tt_atomic_init(&atomic, 0); }

void tearDown() { /* Nothing to clean up */ }

TT_TEST(test_atomic_init) {
  tt_atomic_int_t test_atomic;
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_atomic_init(&test_atomic, 42), "%d");
  TT_ASSERT_EQUAL(42, tt_atomic_load(&test_atomic, TT_MEMORY_ORDER_RELAXED),
                  "%d");
  return true;
}

TT_TEST(test_atomic_store_load) {
  tt_atomic_store(&atomic, 123, TT_MEMORY_ORDER_RELAXED);
  TT_ASSERT_EQUAL(123, tt_atomic_load(&atomic, TT_MEMORY_ORDER_RELAXED), "%d");
  return true;
}

TT_TEST(test_atomic_add) {
  tt_atomic_store(&atomic, 100, TT_MEMORY_ORDER_RELAXED);
  int32_t prev = tt_atomic_add(&atomic, 50, TT_MEMORY_ORDER_RELAXED);
  TT_ASSERT_EQUAL(100, prev, "%d");
  TT_ASSERT_EQUAL(150, tt_atomic_load(&atomic, TT_MEMORY_ORDER_RELAXED), "%d");
  return true;
}

TT_TEST(test_atomic_sub) {
  tt_atomic_store(&atomic, 100, TT_MEMORY_ORDER_RELAXED);
  int32_t prev = tt_atomic_sub(&atomic, 70, TT_MEMORY_ORDER_RELAXED);
  TT_ASSERT_EQUAL(100, prev, "%d");
  TT_ASSERT_EQUAL(30, tt_atomic_load(&atomic, TT_MEMORY_ORDER_RELAXED), "%d");
  return true;
}

TT_TEST(test_atomic_compare_exchange_success) {
  tt_atomic_store(&atomic, 100, TT_MEMORY_ORDER_RELAXED);
  int32_t expected = 100;
  bool success = tt_atomic_compare_exchange(&atomic, &expected, 200,
                                            TT_MEMORY_ORDER_RELAXED);
  TT_ASSERT(success);
  TT_ASSERT_EQUAL(200, tt_atomic_load(&atomic, TT_MEMORY_ORDER_RELAXED), "%d");
  return true;
}

TT_TEST(test_atomic_compare_exchange_failure) {
  tt_atomic_store(&atomic, 100, TT_MEMORY_ORDER_RELAXED);
  int32_t expected = 999;
  bool success = tt_atomic_compare_exchange(&atomic, &expected, 200,
                                            TT_MEMORY_ORDER_RELAXED);
  TT_ASSERT(!success);
  TT_ASSERT_EQUAL(100, tt_atomic_load(&atomic, TT_MEMORY_ORDER_RELAXED), "%d");
  TT_ASSERT_EQUAL(100, expected, "%d");
  return true;
}

int main() {
  TT_TEST_START("Atomic Operations Test Suite");

  TT_SET_FIXTURES(setUp, tearDown);

  TT_RUN_TEST(test_atomic_init);
  TT_RUN_TEST(test_atomic_store_load);
  TT_RUN_TEST(test_atomic_add);
  TT_RUN_TEST(test_atomic_sub);
  TT_RUN_TEST(test_atomic_compare_exchange_success);
  TT_RUN_TEST(test_atomic_compare_exchange_failure);

  TT_TEST_END();
  return 0;
}
