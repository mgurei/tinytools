/**
 * @file test_thread.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29 19:27:52 UTC
 * @brief Thread API test suite
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_atomic.h"
#include "tt_mutex.h"
#include "tt_platform.h"
#include "tt_test.h"
#include "tt_thread.h"
#include "tt_thread_internal.h"
#include <stdio.h>
#include <stdlib.h>

// DEBUG PRINTS ON/OFF
#define TEST_THREAD_PRINT 0

#if defined(TT_CAP_THREADS)
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
  return (void *)((uintptr_t)ITERATIONS);
}

static void *test_sleep_function(void *arg) {
  uint32_t sleep_time = *(uint32_t *)arg;
  tt_thread_sleep(sleep_time);
  return (void *)((uintptr_t)sleep_time);
}

/* Test Setup */
static void test_setup(void) {
  tt_error_t result = tt_thread_init();
  if (result != TT_SUCCESS) {
    fprintf(stderr, "Failed to initialize thread subsystem\n");
    exit(1);
  }
}

#if (TEST_THREAD_PRINT == 1)
void tt_thread_table_print_status(void) {
  printf("\nThread Table Status\n");
  printf("----------------------------------------\n");
  printf("Index | In Use |    Thread Ptr    | State\n");
  printf("----------------------------------------\n");

  tt_mutex_lock(&g_thread_table_mutex);

  for (size_t i = 0; i < TT_MAX_THREADS; i++) {
    if (g_thread_table[i].in_use) {
      printf("[%3zu] |   YES   | %p | %d\n", i,
             (void *)&g_thread_table[i].thread,
             g_thread_table[i].thread->state);
    }
  }

  tt_mutex_unlock(&g_thread_table_mutex);
  printf("----------------------------------------\n\n");
}
#endif /* TEST_THREAD_PRINT */

/* Test cases */
TT_TEST(test_thread_attr_init) {
  test_setup();

  tt_thread_attr_t attr;
  tt_error_t result = tt_thread_attr_init(&attr);

  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT_EQUAL(TT_THREAD_PRIORITY_NORMAL, attr.priority, "%d");
  TT_ASSERT(attr.stack_size > 0);
  TT_ASSERT(attr.name != NULL);
  return true;
}

TT_TEST(test_thread_create_destroy) {
  test_setup();

  tt_thread_t *thread;
  void *retval;
  tt_error_t result = tt_thread_create(&thread, NULL, increment_counter, NULL);

#if (TEST_THREAD_PRINT == 1)
  printf("\nAfter thread creation:\n");
  printf("Direct thread state: %d\n", thread->state);
  tt_thread_table_print_status();
#endif /* TEST_THREAD_PRINT */

  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT(thread != NULL);

  result = tt_thread_join(thread, &retval);

#if (TEST_THREAD_PRINT == 1)
  printf("\nAfter thread join:\n");
  printf("Direct thread state: %d\n", thread->state);
  tt_thread_table_print_status();
#endif /* TEST_THREAD_PRINT */

  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT_EQUAL(ITERATIONS, (int)((uintptr_t)retval), "%d");

#if (TEST_THREAD_PRINT == 1)
  printf("\nBefore thread destroy:\n");
  printf("Direct thread state: %d\n", thread->state);
  tt_thread_table_print_status();
#endif /* TEST_THREAD_PRINT */

  result = tt_thread_destroy(thread);

#if (TEST_THREAD_PRINT == 1)
  printf("\nAfter thread creation:\n");
  printf("Direct thread state: %d\n", thread->state);
  tt_thread_table_print_status();
#endif /* TEST_THREAD_PRINT */

  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  return true;
}

TT_TEST(test_thread_concurrent_execution) {
  test_setup();

  tt_thread_t *threads[NUM_THREADS];
  void *retvals[NUM_THREADS];
  tt_atomic_init(&shared_counter, 0);

  // Create multiple threads
  for (int i = 0; i < NUM_THREADS; i++) {
    tt_error_t result =
        tt_thread_create(&threads[i], NULL, increment_counter, NULL);
    TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  }

  // Join all threads
  for (int i = 0; i < NUM_THREADS; i++) {
    tt_error_t result = tt_thread_join(threads[i], &retvals[i]);
    TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
    TT_ASSERT_EQUAL(ITERATIONS, (int)((uintptr_t)retvals[i]), "%d");
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
  test_setup();

  tt_thread_t *thread;
  uint32_t sleep_time = 100; // 100ms
  void *retval;

  tt_error_t result =
      tt_thread_create(&thread, NULL, test_sleep_function, &sleep_time);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");

  result = tt_thread_join(thread, &retval);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT_EQUAL(sleep_time, (uint32_t)((uintptr_t)retval), "%u");

  tt_thread_destroy(thread);
  return true;
}

TT_TEST(test_thread_priority) {
  test_setup();

  tt_thread_t *thread;
  void *retval;
  tt_error_t result = tt_thread_create(&thread, NULL, increment_counter, NULL);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");

  result = tt_thread_set_priority(thread, TT_THREAD_PRIORITY_HIGH);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");

  result = tt_thread_join(thread, &retval);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT_EQUAL(ITERATIONS, (int)((uintptr_t)retval), "%d");

  tt_thread_destroy(thread);
  return true;
}

TT_TEST(test_thread_state) {
  test_setup();

  tt_thread_t *thread;
  tt_thread_state_t state;
  void *retval;

  tt_error_t result = tt_thread_create(&thread, NULL, increment_counter, NULL);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");

  result = tt_thread_get_state(thread, &state);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT(state == TT_THREAD_STATE_RUNNING ||
            state == TT_THREAD_STATE_CREATED);

  result = tt_thread_join(thread, &retval);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT_EQUAL(ITERATIONS, (int)((uintptr_t)retval), "%d");

  result = tt_thread_get_state(thread, &state);
  TT_ASSERT_EQUAL(TT_SUCCESS, result, "%d");
  TT_ASSERT_EQUAL(TT_THREAD_STATE_TERMINATED, state, "%d");

  tt_thread_destroy(thread);
  return true;
}

#endif /* TT_CAP_THREADS */
/* Test suite main function */
int main(void) {
  TT_TEST_START("Thread API Test Suite");

#if defined(TT_CAP_THREADS)
  TT_RUN_TEST(test_thread_attr_init);
  TT_RUN_TEST(test_thread_create_destroy);
  TT_RUN_TEST(test_thread_concurrent_execution);
  TT_RUN_TEST(test_thread_sleep);
  TT_RUN_TEST(test_thread_priority);
  TT_RUN_TEST(test_thread_state);
#else
  printf("Thread capability inactive. Add -DTT_CAP_THREADS to activate it.\n");
#endif /* TT_CAP_THREADS */

  TT_TEST_END();
  return 0;
}
