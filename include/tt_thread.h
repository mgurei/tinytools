/**
 * @file tt_thread.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_THREAD_H_
#define TT_THREAD_H_

#include "tt_types.h"
#include <stdint.h>

/**
 * @brief Thread priority levels
 */
typedef enum {
  TT_THREAD_PRIORITY_LOW = 0,
  TT_THREAD_PRIORITY_NORMAL,
  TT_THREAD_PRIORITY_HIGH,
  TT_THREAD_PRIORITY_REALTIME
} tt_thread_priority_t;

/**
 * @brief Thread state
 */
typedef enum {
  TT_THREAD_STATE_CREATED = 0,
  TT_THREAD_STATE_RUNNING,
  TT_THREAD_STATE_SUSPENDED,
  TT_THREAD_STATE_TERMINATED
} tt_thread_state_t;

/**
 * @brief Thread attributes
 */
typedef struct {
  tt_thread_priority_t priority;
  size_t stack_size;
  const char *name;
} tt_thread_attr_t;

/**
 * @brief Thread handle
 */
typedef struct tt_thread_t tt_thread_t;

/**
 * @brief Thread function prototype
 */
typedef void *(*tt_thread_func_t)(void *arg);

/**
 * @brief Initialize thread attributes with default values
 * @param attr Pointer to thread attributes structure
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_thread_attr_init(tt_thread_attr_t *attr);

/**
 * @brief Create a new thread
 *
 * Detailed description
 *
 * @param thread Pointer to thread handle
 * @param attr Thread attributes (NULL for defaults)
 * @param func Thread function to execute
 * @param arg Argument passed to thread function
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_thread_create(tt_thread_t **thread, const tt_thread_attr_t *attr,
                            tt_thread_func_t func, void *arg);

/**
 * @brief Join with a terminated thread
 * @param thread Thread handle
 * @param retval Pointer to store thread return value
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_thread_join(tt_thread_t *thread, void **retval);

/**
 * @brief Get current thread state
 * @param thread Thread handle
 * @param state Pointer to store thread state
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_thread_get_state(const tt_thread_t *thread,
                               tt_thread_state_t *state);

/**
 * @brief Set thread priority
 * @param thread Thread handle
 * @param priority New priority level
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_thread_set_priority(tt_thread_t *thread,
                                  tt_thread_priority_t priority);
/**
 * @brief Suspend thread execution
 * @param thread Thread handle
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_thread_suspend(tt_thread_t *thread);

/**
 * @brief Resume thread execution
 * @param thread Thread handle
 * @return TT_SUCCESS on success, error code otherwisw
 */
tt_error_t tt_thread_resume(tt_thread_t *thread);

/**
 * @brief Sleep current thread for specified milliseconds
 * @param ms Time to sleep in milliseconds
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_thread_sleep(uint32_t ms);

/**
 * @brief Get current thread handle
 * @return Current thread handle or NULL on error
 */
tt_thread_t *tt_thread_self(void);

/**
 * @brief Clean up thread resources
 * @param thread Thread handle
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_thread_destroy(tt_thread_t *thread);

#endif // TT_THREAD_H_