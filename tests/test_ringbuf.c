/**
 * @file test_ringbuf.c
 * @brief Test ring buffer functionality
 * @author mgurei
 * @date 2024-12-29
 */

#include "tt_error.h"
#include "tt_ringbuf.h"
#include "tt_test.h"
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE 16
static tt_ringbuf_t rb;
static uint8_t buffer[16];
tt_error_t err;

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

TT_TEST(test_buffer_initial_state) {
  TT_ASSERT(tt_ringbuf_is_empty(&rb));
  TT_ASSERT(!tt_ringbuf_is_full(&rb));
  return true;
}

TT_TEST(test_buffer_write_read) {
  uint8_t test_data = 0x42;
  uint8_t read_data;

  TT_ASSERT_EQUAL(TT_SUCCESS, tt_ringbuf_write(&rb, test_data), "%d");
  TT_ASSERT(!tt_ringbuf_is_empty(&rb));

  TT_ASSERT_EQUAL(TT_SUCCESS, tt_ringbuf_read(&rb, &read_data), "%d");
  TT_ASSERT_EQUAL(test_data, read_data, "0x%02X");
  TT_ASSERT(tt_ringbuf_is_empty(&rb));
  return true;
}

TT_TEST(test_buffer_full) {
  // Fill the buffer
  for (size_t i = 0; i < sizeof(buffer); i++) {
    TT_ASSERT_EQUAL(TT_SUCCESS, tt_ringbuf_write(&rb, (uint8_t)i), "%d");
  }

  TT_ASSERT(tt_ringbuf_is_full(&rb));
  TT_ASSERT_EQUAL(TT_ERROR_BUFFER_FULL, tt_ringbuf_write(&rb, 0xFF), "%d");
  return true;
}

int main(void) {
  TT_TEST_START("Ring Buffer Test Suite");

  TT_SET_FIXTURES(setUp, tearDown);

  TT_RUN_TEST(test_buffer_initial_state);
  TT_RUN_TEST(test_buffer_write_read);
  TT_RUN_TEST(test_buffer_full);

  TT_TEST_END();
  return 0;
}
