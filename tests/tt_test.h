/**
 * @file tt_test.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief Simple test framework for embedded systems
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_TEST_H_
#define TT_TEST_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Test statistics */
typedef struct {
  int total;
  int passed;
  int failed;
  const char *current_suite;
} tt_test_stats_t;

static tt_test_stats_t tt_test_stats = {0, 0, 0, NULL};

/* Test fixture functions */
typedef void (*tt_setup_fn)(void);
typedef void (*tt_teardown_fn)(void);

static tt_setup_fn current_setup = NULL;
static tt_teardown_fn current_teardown = NULL;

/* Colors for test output */
#define TT_COLOR_RED "\x1b[31m"
#define TT_COLOR_GREEN "\x1b[32m"
#define TT_COLOR_YELLOW "\x1b[33m"
#define TT_COLOR_RESET "\x1b[0m"

/**
 * @brief Assert macro with line number and file information
 */
#define TT_ASSERT(condition)                                                   \
  do {                                                                         \
    if (!(condition)) {                                                        \
      printf(TT_COLOR_RED                                                      \
             "Assertion failed at %s:%d: " #condition TT_COLOR_RESET "\n",     \
             __FILE__, __LINE__);                                              \
      return false;                                                            \
    }                                                                          \
  } while (0)

/**
 * @brief Assert equality of two values with custom message
 */
#define TT_ASSERT_EQUAL(expected, actual, fmt)                                 \
  do {                                                                         \
    if ((expected) != (actual)) {                                              \
      printf(TT_COLOR_RED "Assertion failed at %s:%d:\n  Expected: " fmt       \
                          "\n  Actual: " fmt TT_COLOR_RESET "\n",              \
             __FILE__, __LINE__, (expected), (actual));                        \
      return false;                                                            \
    }                                                                          \
  } while (0)

/**
 * @brief Assert string equality
 */
#define TT_ASSERT_STR_EQUAL(expected, actual)                                  \
  do {                                                                         \
    if (strcmp((expected), (actual)) != 0) {                                   \
      printf(TT_COLOR_RED                                                      \
             "String assertion failed at %s:%d:\n  Expected: \"%s\"\n  "       \
             "Actual: \"%s\"" TT_COLOR_RESET "\n",                             \
             __FILE__, __LINE__, (expected), (actual));                        \
      return false;                                                            \
    }                                                                          \
  } while (0)

/**
 * @brief Define a test function
 */
#define TT_TEST(name) bool name(void)

/**
 * @brief Set up and tear down functions for the test suite
 */
#define TT_SET_FIXTURES(setup, teardown)                                       \
  do {                                                                         \
    current_setup = (setup);                                                   \
    current_teardown = (teardown);                                             \
  } while (0)

/**
 * @brief Run a test and update statistics
 */
#define TT_RUN_TEST(test)                                                      \
  do {                                                                         \
    printf("Running " #test "...");                                            \
    fflush(stdout);                                                            \
    tt_test_stats.total++;                                                     \
    bool test_result = true;                                                   \
    if (current_setup) {                                                       \
      current_setup();                                                         \
    }                                                                          \
    test_result = test();                                                      \
    if (current_teardown) {                                                    \
      current_teardown();                                                      \
    }                                                                          \
    if (test_result) {                                                         \
      printf(TT_COLOR_GREEN " PASSED\n" TT_COLOR_RESET);                       \
      tt_test_stats.passed++;                                                  \
    } else {                                                                   \
      printf(TT_COLOR_RED " FAILED\n" TT_COLOR_RESET);                         \
      tt_test_stats.failed++;                                                  \
    }                                                                          \
  } while (0)

/**
 * @brief Start test suite
 */
#define TT_TEST_START(name)                                                    \
  do {                                                                         \
    tt_test_stats.current_suite = name;                                        \
    printf(TT_COLOR_YELLOW "\nRunning test suite: %s\n" TT_COLOR_RESET, name); \
    printf("----------------------------------------\n");                      \
  } while (0)

/**
 * @brief End test suite and print summary
 */
#define TT_TEST_END()                                                          \
  do {                                                                         \
    printf("----------------------------------------\n");                      \
    printf("Test Suite: %s - Summary:\n", tt_test_stats.current_suite);        \
    printf("  Total:  %d\n", tt_test_stats.total);                             \
    if (tt_test_stats.passed > 0) {                                            \
      printf("  Passed: " TT_COLOR_GREEN "%d\n" TT_COLOR_RESET,                \
             tt_test_stats.passed);                                            \
    } else {                                                                   \
      printf("  Passed: %d\n", tt_test_stats.passed);                          \
    }                                                                          \
    if (tt_test_stats.failed > 0) {                                            \
      printf("  Failed: " TT_COLOR_RED "%d\n" TT_COLOR_RESET,                  \
             tt_test_stats.failed);                                            \
    } else {                                                                   \
      printf("  Failed: %d\n", tt_test_stats.failed);                          \
    }                                                                          \
    return tt_test_stats.failed;                                               \
  } while (0)

#endif /* TT_TEST_H_ */
