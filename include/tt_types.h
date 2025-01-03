/**
 * @file tt_types.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-28
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_TYPES_H_
#define TT_TYPES_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Error codes for TinyTools library
 */
typedef enum {
  TT_SUCCESS = 0,             /**< Operation completed successfully*/
  TT_ERROR_NULL_POINTER,      /**< Null pointer was passed*/
  TT_ERROR_INVALID_PARAM,     /**< Invalid parameter value*/
  TT_ERROR_BUFFER_FULL,       /**< Buffer is full */
  TT_ERROR_BUFFER_EMPTY,      /**< Buffer is empy*/
  TT_ERROR_MEMORY,            /**< Memory allocation failed*/
  TT_ERROR_TIMEOUT,           /**< Operation timed out*/
  TT_ERROR_BUSY,              /**< Resource is busy*/
  TT_ERROR_NOT_INITIALIZED,   /**< Module not initialized*/
  TT_ERROR_PLATFORM_SPECIFIC, /**< Platform-specific error*/
  TT_ERROR_MUTEX_ERROR,       /**< Mutex operation failed*/
  // 10
  TT_ERROR_MUTEX_INIT,      /**< Mutex initialization failed*/
  TT_ERROR_MUTEX_DESTROY,   /**< Mutex destroy failed*/
  TT_ERROR_MUTEX_LOCK,      /**< Failed to lock mutex*/
  TT_ERROR_MUTEX_UNLOCK,    /**< Failed to unlock mutex*/
  TT_ERROR_NOT_FOUND,       /**< Resource not found*/
  TT_ERROR_THREAD_CREATE,   /**< Fail to create thread*/
  TT_ERROR_THREAD_JOIN,     /**< Fail to join thread*/
  TT_ERROR_THREAD_PRIORITY, /**< Fail to set thread priority */
  TT_ERROR_THREAD_SLEEP,    /**< Fail to set thread to sleep*/
  TT_ERROR_THREAD_ACTIVE,   /**< Fail to destroy a thread*/
  // 20
  TT_ERROR_THREAD_DETACH,          /**< Fail to detach thread*/
  TT_ERROR_PLATFORM_INIT,          /**< Fail to initialized platform*/
  TT_ERROR_ALREADY_INITIALIZED,    /**< Resource is already initialized*/
  TT_ERROR_PLATFORM_NOT_SUPPORTED, /**< Platform not supported*/

  TT_ERROR_NOT_IMPLEMENTED, /**< Feature not implemented*/
  TT_ERROR_UNKNOWN,         /**< Unknown error occured*/
  // Add more error codes as needed
} tt_error_t;

#endif // TT_TYPES_H_
