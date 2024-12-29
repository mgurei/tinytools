/**
 * @file test_error.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief Error handling test suite
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_error.h"
#include "tt_test.h"

static tt_error_t last_callback_error = TT_SUCCESS;

void setUp(void) {
  tt_error_t result = tt_error_init();
  if (result != TT_SUCCESS) {
    printf("Error initializing error handling: %s\n",
           tt_error_to_string(result));
  }
}

void tearDown(void) { tt_error_deinit(); }

static void error_handle_callback(tt_error_t error) {
  last_callback_error = error;
}

TT_TEST(test_error_deinit) {
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_error_deinit(), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NOT_INITIALIZED, tt_error_get_last(), "%d");
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_error_init(), "%d");
  TT_ASSERT(tt_error_is_success());
  return true;
}

TT_TEST(test_error_string_conversion) {
  TT_ASSERT_STR_EQUAL("Success", tt_error_to_string(TT_SUCCESS));
  TT_ASSERT_STR_EQUAL("Null pointer",
                      tt_error_to_string(TT_ERROR_NULL_POINTER));
  TT_ASSERT_STR_EQUAL("Unknown error", tt_error_to_string(999));
  return true;
}

TT_TEST(test_error_callback) {
  last_callback_error = TT_SUCCESS;

  // Register callback
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_error_register_callback(error_handle_callback),
                  "%d");
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_error_set_last(TT_ERROR_BUFFER_FULL), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_BUFFER_FULL, last_callback_error, "%d");
  TT_ASSERT_EQUAL(TT_ERROR_BUFFER_FULL, tt_error_get_last(), "%d");
  return true;
}

TT_TEST(test_error_clear) {
  /* Set an error first */
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_error_set_last(TT_ERROR_BUFFER_FULL), "%d");
  TT_ASSERT(!tt_error_is_success());

  /* Clear the error and verify */
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_error_clear(), "%d");
  TT_ASSERT(tt_error_is_success());
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_error_get_last(), "%d");
  return true;
}

TT_TEST(test_error_uninitialized) {
  TT_ASSERT_EQUAL(TT_SUCCESS, tt_error_deinit(), "%d");

  TT_ASSERT_EQUAL(TT_ERROR_NOT_INITIALIZED,
                  tt_error_set_last(TT_ERROR_BUFFER_FULL), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NOT_INITIALIZED, tt_error_get_last(), "%d");
  TT_ASSERT_EQUAL(TT_ERROR_NOT_INITIALIZED,
                  tt_error_register_callback(error_handle_callback), "%d");

  TT_ASSERT_EQUAL(TT_SUCCESS, tt_error_init(), "%d");
  return true;
}

int main(void) {
  TT_TEST_START("Error Handling Test Suite");

  TT_SET_FIXTURES(setUp, tearDown);

  TT_RUN_TEST(test_error_deinit);
  TT_RUN_TEST(test_error_string_conversion);
  TT_RUN_TEST(test_error_callback);
  TT_RUN_TEST(test_error_clear);
  TT_RUN_TEST(test_error_uninitialized);

  TT_TEST_END();
  return 0;
}
