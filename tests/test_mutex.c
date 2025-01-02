/**
 * @file test_mutex.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_error.h"
#include "tt_mutex.h"
#include "tt_test.h"
#include <stddef.h>

#if defined(TT_CAP_MUTEX)
static tt_mutex_t test_mutex;

void setUp(void) { tt_mutex_init(&test_mutex); }

void tearDown(void) { tt_mutex_destroy(&test_mutex); }

TT_TEST(test_mutex_init) {
  tt_mutex_t mutex;
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_init(&mutex), "%d");
  TT_ASSERT(!tt_mutex_is_locked(&mutex));
  return true;
}

TT_TEST(test_mutex_null_pointer) {
  TT_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_init(NULL), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_lock(NULL), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_unlock(NULL), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_trylock(NULL), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_destroy(NULL), "%d");
  TT_ASSERT(!tt_mutex_is_locked(NULL));
  return true;
}

TT_TEST(test_mutex_lock_unlock) {
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_lock(&test_mutex), "%d");
  TT_ASSERT(tt_mutex_is_locked(&test_mutex));
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_unlock(&test_mutex), "%d");
  TT_ASSERT(!tt_mutex_is_locked(&test_mutex));
  return true;
}

TT_TEST(test_mutex_trylock) {
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_trylock(&test_mutex), "%d");
  TT_ASSERT(tt_mutex_is_locked(&test_mutex));
  TT_ASSERT_EQUAL(TT_ERROR_BUSY, tt_mutex_trylock(&test_mutex), "%d");
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_unlock(&test_mutex), "%d");
  TT_ASSERT(!tt_mutex_is_locked(&test_mutex));
  return true;
}

#else  /* General non platform specific mutex implementation */
static tt_mutex_t test_mutex;

void setUp(void) { tt_mutex_init(&test_mutex); }

void tearDown(void) { tt_mutex_destroy(&test_mutex); }

TT_TEST(test_mutex_init) {
  tt_mutex_t mutex;
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_init(&mutex), "%d");
  TT_ASSERT(!tt_mutex_is_locked(&mutex));
  return true;
}

TT_TEST(test_mutex_null_pointer) {
  TT_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_init(NULL), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_lock(NULL), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_unlock(NULL), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_trylock(NULL), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NULL_POINTER, tt_mutex_destroy(NULL), "%d");
  TT_ASSERT(!tt_mutex_is_locked(NULL));
  return true;
}

TT_TEST(test_mutex_lock_unlock) {
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_lock(&test_mutex), "%d");
  TT_ASSERT(tt_mutex_is_locked(&test_mutex));
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_unlock(&test_mutex), "%d");
  TT_ASSERT(!tt_mutex_is_locked(&test_mutex));
  return true;
}

TT_TEST(test_mutex_trylock) {
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_trylock(&test_mutex), "%d");
  TT_ASSERT(tt_mutex_is_locked(&test_mutex));
  TT_ASSERT_EQUAL(TT_ERROR_BUSY, tt_mutex_trylock(&test_mutex), "%d");
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_mutex_unlock(&test_mutex), "%d");
  TT_ASSERT(!tt_mutex_is_locked(&test_mutex));
  return true;
}
#endif /* TT_CAP_MUTEX */

int main(void) {
  TT_TEST_START("Mutex Test Suite");

  TT_SET_FIXTURES(setUp, tearDown);

#if defined(TT_CAP_MUTEX)
  TT_RUN_TEST(test_mutex_init);
  TT_RUN_TEST(test_mutex_null_pointer);
  TT_RUN_TEST(test_mutex_lock_unlock);
  TT_RUN_TEST(test_mutex_trylock);
#else
  TT_RUN_TEST(test_mutex_init);
  TT_RUN_TEST(test_mutex_null_pointer);
  TT_RUN_TEST(test_mutex_lock_unlock);
  TT_RUN_TEST(test_mutex_trylock);
#endif /* TT_CAP_MUTEX*/

  TT_TEST_END();
  return 0;
}
