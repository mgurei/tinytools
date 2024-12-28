/**
 * @file test_error.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-27
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_error.h"
#include "unity.h"

static tt_error_t last_callback_error = TT_SUCCESS;

void setUp(void) { tt_error_init(); }

void tearDown(void) { tt_error_deinit(); }

static void error_handle_callback(tt_error_t error) {
  last_callback_error = error;
}

void test_error_deinit(void) {
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_error_deinit());
  TEST_ASSERT_EQUAL(TT_ERROR_NOT_INITIALIZED, tt_error_get_last());
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_error_init());
  TEST_ASSERT_TRUE(tt_error_is_success());
}

void test_error_string_conversion(void) {
  // Test that error codes convert to correct strings
  TEST_ASSERT_EQUAL_STRING("Success", tt_error_to_string(TT_SUCCESS));
  TEST_ASSERT_EQUAL_STRING("Null pointer",
                           tt_error_to_string(TT_ERROR_NULL_POINTER));
  TEST_ASSERT_EQUAL_STRING("Unknown error", tt_error_to_string(999));
}

void test_error_callback(void) {
  last_callback_error = TT_SUCCESS;

  // Register callback
  TEST_ASSERT_EQUAL(TT_SUCCESS,
                    tt_error_register_callback(error_handle_callback));
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_error_set_last(TT_ERROR_BUFFER_FULL));
  TEST_ASSERT_EQUAL(TT_ERROR_BUFFER_FULL, last_callback_error);
  TEST_ASSERT_EQUAL(TT_ERROR_BUFFER_FULL, tt_error_get_last());
}

void test_error_clear(void) {
  /* Set an error first */
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_error_set_last(TT_ERROR_BUFFER_FULL));
  TEST_ASSERT_FALSE(tt_error_is_success());

  /* Clear the error and verify */
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_error_clear());
  TEST_ASSERT_TRUE(tt_error_is_success());
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_error_get_last());
}

void test_error_uninitialized(void) {
  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_error_deinit());

  TEST_ASSERT_EQUAL(TT_ERROR_NOT_INITIALIZED,
                    tt_error_set_last(TT_ERROR_BUFFER_FULL));
  TEST_ASSERT_EQUAL(TT_ERROR_NOT_INITIALIZED, tt_error_get_last());
  TEST_ASSERT_EQUAL(TT_ERROR_NOT_INITIALIZED,
                    tt_error_register_callback(error_handle_callback));

  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_error_init());
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_error_deinit);
  RUN_TEST(test_error_string_conversion);
  RUN_TEST(test_error_callback);
  RUN_TEST(test_error_clear);
  return UNITY_END();
}
