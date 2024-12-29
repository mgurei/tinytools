/**
 * @file test_thread.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief Thread API test suite
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_atomic.h"
#include "tt_test.h"
#include "tt_thread.h"
#include <stdio.h>
#include <stdlib.h>

/* Test data structures */
static tt_atomic_int_t shared_counter;
static const int ITERATIONS = 1000;
static const int NUM_THREADS = 4;

/* Test helper functions */
static void *increment_counter(void *arg) {
  (void)arg; // Suppress unused parameter warning
  for (int i = 0; i < ITERATIONS; i++) {
    tt_atomic_add(&shared_counter, 1, TT_MEMORY_ORDER_RELEASE);
  }
  return NULL;
}

static void *test_sleep_function(void *arg) {
  uint32_t sleep_time = *(uint32_t *)arg;
  tt_thread_sleep(sleep_time);
  return NULL;
}

/* Test cases */
TT_TEST(test_thread_attr_init) {
  tt_thread_attr_t attr;
  tt_error_t result = tt_thread_attr_init(&attr);

  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT_EQUAL(TT_THREAD_PRIORITY_NORMAL, attr.priority, "%d");
  TT_ASSERT(attr.stack_size > 0);
  TT_ASSERT(attr.name != NULL);
  return true;
}

TT_TEST(test_thread_create_destroy) {
  tt_thread_t *thread;
  tt_error_t result = tt_thread_create(&thread, NULL, increment_counter, NULL);

  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT(thread != NULL);

  result = tt_thread_join(thread, NULL);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");

  result = tt_thread_destroy(thread);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  return true;
}

TT_TEST(test_thread_concurrent_execution) {
  tt_thread_t *threads[NUM_THREADS];
  tt_atomic_init(&shared_counter, 0);

  // Create multiple threads
  for (int i = 0; i < NUM_THREADS; i++) {
    tt_error_t result =
        tt_thread_create(&threads[i], NULL, increment_counter, NULL);
    TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  }

  // Join all threads
  for (int i = 0; i < NUM_THREADS; i++) {
    tt_error_t result = tt_thread_join(threads[i], NULL);
    TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  }

  // Add memory fence to ensure all threads have completed
  tt_atomic_thread_fence(TT_MEMORY_ORDER_ACQUIRE);

  // Verify final counter value
  int final_count = tt_atomic_load(&shared_counter, TT_MEMORY_ORDER_RELAXED);
  TT_ASSERT_EQUAL(NUM_THREADS * ITERATIONS, final_count, "%d");

  // Cleanup
  for (int i = 0; i < NUM_THREADS; i++) {
    tt_thread_destroy(threads[i]);
  }
  return true;
}

TT_TEST(test_thread_sleep) {
  tt_thread_t *thread;
  uint32_t sleep_time = 100; // 100ms

  tt_error_t result =
      tt_thread_create(&thread, NULL, test_sleep_function, &sleep_time);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");

  result = tt_thread_join(thread, NULL);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");

  tt_thread_destroy(thread);
  return true;
}

TT_TEST(test_thread_priority) {
  tt_thread_t *thread;
  tt_error_t result = tt_thread_create(&thread, NULL, increment_counter, NULL);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");

  result = tt_thread_set_priority(thread, TT_THREAD_PRIORITY_HIGH);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");

  tt_thread_join(thread, NULL);
  tt_thread_destroy(thread);
  return true;
}

TT_TEST(test_thread_state) {
  tt_thread_t *thread;
  tt_thread_state_t state;

  tt_error_t result = tt_thread_create(&thread, NULL, increment_counter, NULL);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");

  result = tt_thread_get_state(thread, &state);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT(state == TT_THREAD_STATE_RUNNING ||
            state == TT_THREAD_STATE_CREATED);

  tt_thread_join(thread, NULL);
  result = tt_thread_get_state(thread, &state);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT_EQUAL(TT_THREAD_STATE_TERMINATED, state, "%d");

  tt_thread_destroy(thread);
  return true;
}

/* Test suite main function */
int main(void) {
  TT_TEST_START("Thread API Test Suite");

  TT_RUN_TEST(test_thread_attr_init);
  TT_RUN_TEST(test_thread_create_destroy);
  TT_RUN_TEST(test_thread_concurrent_execution);
  TT_RUN_TEST(test_thread_sleep);
  TT_RUN_TEST(test_thread_priority);
  TT_RUN_TEST(test_thread_state);

  TT_TEST_END();
  return 0;
}
