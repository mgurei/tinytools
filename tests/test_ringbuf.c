/**
 * @file test_ringbuf.c
 * @brief Test ring buffer functionality
 * @author mgurei
 * @date 2024-12-27
 */

#include "tt_ringbuf.h"
#include <stdio.h>

#define BUFFER_SIZE 5

int main(void) {
  uint8_t buffer[BUFFER_SIZE];
  tt_ringbuf_t rb;
  uint8_t data;
  tt_error_t err;

  printf("Testing ring buffer...\n");

  // Initialize
  err = tt_ringbuf_init(&rb, buffer, BUFFER_SIZE);
  if (err != TT_SUCCESS) {
    printf("Failed to initialize ring buffer: %s\n", tt_error_to_string(err));
    return 1;
  }

  // Test writing
  printf("Writing to buffer: ");
  for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
    err = tt_ringbuf_write(&rb, i);
    if (err != TT_SUCCESS) {
      printf("\nWrite failed: %s\n", tt_error_to_string(err));
      return 1;
    }
    printf("%d ", i);
  }
  printf("\n");

  // Test buffer full condition
  err = tt_ringbuf_write(&rb, 42);
  if (err != TT_ERROR_BUFFER_FULL) {
    printf("Expected buffer full error\n");
    return 1;
  }

  // Test reading
  printf("Reading from buffer: ");
  while (!tt_ringbuf_is_empty(&rb)) {
    err = tt_ringbuf_read(&rb, &data);
    if (err != TT_SUCCESS) {
      printf("\nRead failed: %s\n", tt_error_to_string(err));
      return 1;
    }
    printf("%d ", data);
  }
  printf("\n");

  printf("All tests passed!\n");
  return 0;
}
