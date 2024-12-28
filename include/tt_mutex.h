/**
 * @file tt_mutex.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-28
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_MUTEX_H_
#define TT_MUTEX_H_

#include "tt_error.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Mutex structure for thread synchronization
 */
typedef struct {
  volatile int lock; /**< Atomic lock value*/
  bool initialized;  /**< Initializatio state*/
} tt_mutex_t;

/**
 * @brief Initialize a mutex
 * @param mutex Pointer to mutex structure
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_mutex_init(tt_mutex_t *mutex);

/**
 * @brief Lock a mutex
 * @param mutex Pointer to mutex structure
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_mutex_lock(tt_mutex_t *mutex);

/**
 * @brief Unlock a mutex
 * @param mutex Pointer to mutex structure
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_mutex_unlock(tt_mutex_t *mutex);

/**
 * @brief Try to lock a mutex without blocking
 * @param mutex Pointer to mutex structure
 * @return TT_SUCCESS on success, TT_ERROR_BUSY if mutex is already locked
 */
tt_error_t tt_mutex_trylock(tt_mutex_t *mutex);

/**
 * @brief Check if mutex is locked
 * @param mutex Pointer to mutex structure
 * @return true if mutex is locked, false otherwise
 */
bool tt_mutex_is_locked(tt_mutex_t *mutex);

#endif // TT_MUTEX_H_
