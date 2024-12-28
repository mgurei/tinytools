/**
 * @file test_mutex.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-28
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_error.h"
#include "tt_mutex.h"
#include "unity.h"
#include <stddef.h>

static tt_mutex_t test_mutex;

void setUp(void) { tt_mutex_init(&test_mutex); }

void tearDown(void) { /* Nothing to clean up */ }

void test_mutex_init(void) {
  tt_mutex_t mutex;
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_init(&mutex));
  TEST_ASSERT_FALSE(tt_mutex_is_locked(&mutex));
}

void test_mutex_null_pointer(void) {
  TEST_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_init(NULL));
  TEST_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_lock(NULL));
  TEST_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_unlock(NULL));
  TEST_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_trylock(NULL));
  TEST_ASSERT_FALSE(tt_mutex_is_locked(NULL));
}

void test_mutex_lock_unlock(void) {
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_lock(&test_mutex));
  TEST_ASSERT_TRUE(tt_mutex_is_locked(&test_mutex));
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_unlock(&test_mutex));
  TEST_ASSERT_FALSE(tt_mutex_is_locked(&test_mutex));
}

void test_mutex_trylock(void) {
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_trylock(&test_mutex));
  TEST_ASSERT_TRUE(tt_mutex_is_locked(&test_mutex));
  TEST_ASSERT_EQUAL(TT_ERROR_BUSY, tt_mutex_trylock(&test_mutex));
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_unlock(&test_mutex));
  TEST_ASSERT_FALSE(tt_mutex_is_locked(&test_mutex));
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_mutex_init);
  RUN_TEST(test_mutex_null_pointer);
  RUN_TEST(test_mutex_lock_unlock);
  RUN_TEST(test_mutex_trylock);
  return UNITY_END();
}
