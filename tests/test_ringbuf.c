/**
 * @file test_ringbuf.c
 * @brief Test ring buffer functionality
 * @author mgurei
 * @date 2024-12-27
 */

#include "tt_error.h"
#include "tt_ringbuf.h"
#include "unity.h"
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE 16
static tt_ringbuf_t rb;
static uint8_t buffer[16];
tt_error_t err;

/* These functions must be defined even if empty */
void setUp(void) {
  // Initialize
  err = tt_ringbuf_init(&rb, buffer, BUFFER_SIZE);
  if (err != TT_SUCCESS) {
    printf("Failed to initialize ring buffer: %s\n", tt_error_to_string(err));
  }
}

void tearDown(void) {
  // Optional: Clean up after each test
}

void test_buffer_initial_state(void) {
  TEST_ASSERT_TRUE(tt_ringbuf_is_empty(&rb));
  TEST_ASSERT_FALSE(tt_ringbuf_is_full(&rb));
}

void test_buffer_write_read(void) {
  uint8_t test_data = 0x42;
  uint8_t read_data;

  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_ringbuf_write(&rb, test_data));
  TEST_ASSERT_FALSE(tt_ringbuf_is_empty(&rb));

  TEST_ASSERT_EQUAL(TT_SUCCESS, tt_ringbuf_read(&rb, &read_data));
  TEST_ASSERT_EQUAL(test_data, read_data);
  TEST_ASSERT_TRUE(tt_ringbuf_is_empty(&rb));
}

void test_buffer_full(void) {
  // Fill the buffer
  for (size_t i = 0; i < sizeof(buffer); i++) {
    TEST_ASSERT_EQUAL(TT_SUCCESS, tt_ringbuf_write(&rb, (uint8_t)i));
  }

  TEST_ASSERT_TRUE(tt_ringbuf_is_full(&rb));
  TEST_ASSERT_EQUAL(TT_ERROR_BUFFER_FULL, tt_ringbuf_write(&rb, 0xFF));
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_buffer_initial_state);
  RUN_TEST(test_buffer_write_read);
  RUN_TEST(test_buffer_full);

  return UNITY_END();
}
