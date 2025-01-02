/**
 * @file tt_mutex.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-28
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_MUTEX_H_
#define TT_MUTEX_H_

#include "tt_types.h"
#include <stddef.h>

#if defined(TT_CAP_MUTEX)
/**
 * @brief Mutex structure for thread synchronization
 */
typedef struct {
  void *lock;       /**< Pointer to platform specific implementation*/
  bool initialized; /**< Initialization state*/
} tt_mutex_t;

/**
 * @brief Initialize a mutex
 * @param mutex Pointer to mutex structure
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_mutex_init(tt_mutex_t *mutex);
/**
 * @brief Destroy a mutex
 * @param mutex Pointer to mutex structure
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_mutex_destroy(tt_mutex_t *mutex);

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

#else /* General non-platform-specific mutex implementation */

/**
 * @brief Mutex structure for thread synchronization
 */
typedef struct {
  volatile int *lock; /**< Atomic lock value*/
  bool initialized;   /**< Initialization state*/
} tt_mutex_t;

/* Initial lock value */
#define TT_MUTEX_UNLOCKED 0
#define TT_MUTEX_LOCKED 1

tt_error_t tt_mutex_init(tt_mutex_t *mutex);
tt_error_t tt_mutex_destroy(tt_mutex_t *mutex);
tt_error_t tt_mutex_lock(tt_mutex_t *mutex);
tt_error_t tt_mutex_unlock(tt_mutex_t *mutex);
tt_error_t tt_mutex_trylock(tt_mutex_t *mutex);
bool tt_mutex_is_locked(tt_mutex_t *mutex);

#endif /* TT_CAP_MUTEX */

#endif // TT_MUTEX_H_
