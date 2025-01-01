/**
 * @file tt_thread_internal.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_THREAD_INTERNAL_H_
#define TT_THREAD_INTERNAL_H_

#include "tt_mutex.h"
#include "tt_thread.h"

#if defined(TT_PLATFORM_LINUX)

#include <pthread.h>
typedef pthread_t tt_platform_thread_handle_t;

#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)

#include <FreeRTOS.h>
#include <task.h>
typedef TaskHandle_t tt_platform_thread_handle_t;

#else

typedef void *tt_platform_thread_handle_t;

#endif

// Maximum number of threads the can be tracked
#ifndef TT_MAX_THREADS
#define TT_MAX_THREADS 32
#endif

/**
 * @brief Thread structure
 */
struct tt_thread_t {
  tt_platform_thread_handle_t handle; /**< Platform-specifi thread handle*/
  tt_thread_state_t state;            /**< Current thread state*/
  tt_thread_priority_t priority;      /**< Current priority*/
  tt_thread_func_t func;              /**< Thread function*/
  void *arg;                          /**< Thread function argument*/
  const char *name;                   /**< Thread name (if supported)*/
  size_t stack_size;                  /**< Stack size (if applicable)*/
  void *retval;                       /**< Return value*/
  bool is_active;                     /**< Slot is in use*/
};

/**
 * @brief Thread table entry
 */
typedef struct {
  tt_thread_t *thread;
  bool in_use;
} tt_thread_table_entry_t;

/**
 * @brief Global thread table
 */
extern tt_thread_table_entry_t g_thread_table[TT_MAX_THREADS];
extern tt_mutex_t g_thread_table_mutex;

/**
 * @brief Initialize the thread table
 * @return TT_SUCCESS if successful, error code otherwise
 */
tt_error_t tt_thread_table_init(void);

/**
 * @brief Clean up the thread table
 * @return TT_SUCCESS if successful, error code otherwise
 */
tt_error_t tt_thread_table_cleanup(void);

/**
 * @brief Register thread table
 * @param thread Pointer to the thread
 * @return TT_SUCCESS if successful, error code otherwise
 */
tt_error_t tt_thread_table_register(tt_thread_t *thread);

/**
 * @brief Unregister thread table
 * @param thread Pointer to the thread
 * @return TT_SUCCESS if successful, error code otherwise
 */
tt_error_t tt_thread_table_unregister(tt_thread_t *thread);

/**
 * @brief Find thread table by using handle
 * @param handle Platform specific thread handle
 * @return Thread table pointer
 */
tt_thread_t *tt_thread_table_find_by_handle(tt_platform_thread_handle_t handle);

#endif // TT_THREAD_INTERNAL_H_
