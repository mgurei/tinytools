/**
 * @file tt_ringbuf.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-27
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_RINGBUF_H_
#define TT_RINGBUF_H_
#include "tt_error.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Ring buffer structure
 */
typedef struct {
  uint8_t *buffer; /* Pointer to buffer memory */
  size_t size;     /* Buffer size */
  size_t head;     /* Write index */
  size_t tail;     /* Read index */
  size_t count;    /* Number of items in buffer */
} tt_ringbuf_t;

/**
 * @brief Initialize ring buffer
 * @param rb Pointer to ring buffer structure
 * @param buffer Pointer to buffer memory
 * @param size Size of buffer
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_ringbuf_init(tt_ringbuf_t *rb, uint8_t *buffer, size_t size);

/**
 * @brief Write byte to ring buffer
 * @param rb Pointer to ring buffer structure
 * @param data Byte to write
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_ringbuf_write(tt_ringbuf_t *rb, uint8_t data);

/**
 * @brief Read byte from ring buffer
 * @param rb Pointer to ring buffer structure
 * @param data Pointer to store read byte
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_ringbuf_read(tt_ringbuf_t *rb, uint8_t *data);

/**
 * @brief Get number of items in ring buffer
 * @param rb Pointer to ring buffer structure
 * @return Number of items in buffer
 */
size_t tt_ringbuf_count(const tt_ringbuf_t *rb);

/**
 * @brief Check if ring buffer is empty
 * @param rb Pointer to ring buffer structure
 * @return true if empty, false otherwise
 */
bool tt_ringbuf_is_empty(const tt_ringbuf_t *rb);

/**
 * @brief Check if ring buffer is full
 * @param rb Pointer to ring buffer structure
 * @return true if full, false otherwise
 */
bool tt_ringbuf_is_full(const tt_ringbuf_t *rb);

#endif // TT_RINGBUF_H_
