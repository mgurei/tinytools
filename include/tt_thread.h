/**
 * @file tt_thread.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_THREAD_H_
#define TT_THREAD_H_

#include "tt_platform.h"
#include "tt_types.h"

#if defined(TT_CAP_THREADS)
#include "tt_thread_enum.h"
#include "tt_thread_internal.h"

/**
 * @brief Thread attributes
 */
struct tt_thread_attr_t {
  tt_thread_priority_t priority;
  size_t stack_size;
  const char *name;
};

/**
 * @brief Thread handle
 */
typedef struct tt_thread_t tt_thread_t;

/**
 * @brief Thread function prototype
 */
typedef void *(*tt_thread_func_t)(void *arg);

/**
 * @brief Initialize global thread handle
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_thread_init(void);

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

#else

/**
 * @brief Dummy thread struct
 */
struct tt_thread_t {
  int dummy; /**< Dummy field for platforms without thread support*/
};

struct tt_thread_attr_t {
  int dummy; /**< Dummy field for platforms without thread support*/
};

typedef enum { DUMMY_PRIORITY } tt_thread_priority_t;
typedef enum { DUMMY_STATE } tt_thread_state_t;

// Dummy functions
typedef void *(*tt_thread_func_t)(void *arg);

static inline tt_error_t tt_thread_init(void) {
  return TT_ERROR_NOT_IMPLEMENTED;
}
static inline tt_error_t tt_thread_attr_init(tt_thread_attr_t *attr
                                             __attribute__((unused))) {
  return TT_ERROR_NOT_IMPLEMENTED;
}
static inline tt_error_t
tt_thread_create(tt_thread_t **thread __attribute__((unused)),
                 const tt_thread_attr_t *attr __attribute__((unused)),
                 tt_thread_func_t func __attribute__((unused)),
                 void *arg __attribute__((unused))) {
  return TT_ERROR_NOT_IMPLEMENTED;
}
static inline tt_error_t tt_thread_join(tt_thread_t *thread
                                        __attribute__((unused)),
                                        void **retval __attribute__((unused))) {
  return TT_ERROR_NOT_IMPLEMENTED;
}
static inline tt_error_t tt_thread_get_state(const tt_thread_t *thread
                                             __attribute__((unused)),
                                             tt_thread_state_t *state
                                             __attribute__((unused))) {
  return TT_ERROR_NOT_IMPLEMENTED;
}
static inline tt_error_t tt_thread_set_priority(tt_thread_t *thread
                                                __attribute__((unused)),
                                                tt_thread_priority_t priority
                                                __attribute__((unused))) {
  return TT_ERROR_NOT_IMPLEMENTED;
}
static inline tt_error_t tt_thread_suspend(tt_thread_t *thread
                                           __attribute__((unused))) {
  return TT_ERROR_NOT_IMPLEMENTED;
}
static inline tt_error_t tt_thread_resume(tt_thread_t *thread
                                          __attribute__((unused))) {
  return TT_ERROR_NOT_IMPLEMENTED;
}
static inline tt_error_t tt_thread_sleep(uint32_t ms __attribute__((unused))) {
  return TT_ERROR_NOT_IMPLEMENTED;
}
static inline tt_thread_t *tt_thread_self(void) { return NULL; }
static inline tt_error_t tt_thread_destroy(tt_thread_t *thread
                                           __attribute__((unused))) {
  return TT_ERROR_NOT_IMPLEMENTED;
}

#endif /* TT_CAP_THREADS */

#endif // TT_THREAD_H_
