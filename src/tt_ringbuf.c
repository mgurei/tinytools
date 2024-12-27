/**
 * @file tt_ringbuf.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-27
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_ringbuf.h"

tt_error_t tt_ringbuf_init(tt_ringbuf_t *rb, uint8_t *buffer, size_t size) {
  if (!rb || !buffer || !size) {
    return TT_ERROR_INVALID_PARAM;
  }

  rb->buffer = buffer;
  rb->size = size;
  rb->head = 0;
  rb->tail = 0;
  rb->count = 0;

  return TT_SUCCESS;
}

tt_error_t tt_ringbuf_write(tt_ringbuf_t *rb, uint8_t data) {
  if (!rb) {
    return TT_ERROR_NULL_POINTER;
  }

  if (tt_ringbuf_is_full(rb)) {
    return TT_ERROR_BUFFER_FULL;
  }

  rb->buffer[rb->head] = data;
  rb->head = (rb->head + 1) % rb->size;
  rb->count++;

  return TT_SUCCESS;
}

tt_error_t tt_ringbuf_read(tt_ringbuf_t *rb, uint8_t *data) {
  if (!rb || !data) {
    return TT_ERROR_NULL_POINTER;
  }

  if (tt_ringbuf_is_empty(rb)) {
    return TT_ERROR_BUFFER_EMPTY;
  }

  *data = rb->buffer[rb->tail];
  rb->tail = (rb->tail + 1) % rb->size;
  rb->count--;

  return TT_SUCCESS;
}

size_t tt_ringbuf_count(const tt_ringbuf_t *rb) { return rb ? rb->count : 0; }

bool tt_ringbuf_is_empty(const tt_ringbuf_t *rb) {
  return rb ? (rb->count == 0) : true;
}

bool tt_ringbuf_is_full(const tt_ringbuf_t *rb) {
  return rb ? (rb->count == rb->size) : true;
}
