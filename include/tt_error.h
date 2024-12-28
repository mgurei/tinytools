/**
 * @file tt_error.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-27
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_ERROR_H_
#define TT_ERROR_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  TT_SUCCESS = 0,             /**< Operation completed successfully*/
  TT_ERROR_NULL_POINTER,      /**< Null pointer was passed*/
  TT_ERROR_INVALID_PARAM,     /**< Invalid parameter value*/
  TT_ERROR_BUFFER_FULL,       /**< Buffer is full */
  TT_ERROR_BUFFER_EMPTY,      /**< Buffer is empy*/
  TT_ERROR_OUT_OF_MEMORY,     /**< Memory allocation failed*/
  TT_ERROR_TIMEOUT,           /**< Operation timed out*/
  TT_ERROR_BUSY,              /**< Resource is busy*/
  TT_ERROR_NOT_INITIALIZED,   /**< Module not initialized*/
  TT_ERROR_PLATFORM_SPECIFIC, /**< Platform-specific error*/
  TT_ERROR_UNKNOWN,           /**< Unknown error occured*/
  // Add more error codes as needed
} tt_error_t;

/**
 * @brief Initialize the error handling system
 * @return TT_SUCCESS if initialized successfully
 */
tt_error_t tt_error_init(void);

/**
 * @brief Get the last error that occured
 * @return The last error code
 */
tt_error_t tt_error_get_last(void);

/**
 * @brief Set the last error code
 * @param error Error code to convert
 */
void tt_error_set_last(tt_error_t error);

/**
 * @brief Convert error code to string
 * @param error Error code
 * @return String representation of error
 */
const char *tt_error_to_string(tt_error_t error);

/**
 * @brief Check if the last error operation was successful
 * @return true if last was successful, false otherwise
 */
bool tt_error_is_success(void);

/**
 * @brief Clear last error
 */
void tt_error_clear(void);

/**
 * @brief Register a callback for error handling
 * @param callback Function pointer to error handle
 * @return TT_SUCCESS if callback was register successfully
 */
tt_error_t tt_error_register_callback(void (*callback)(tt_error_t));

#endif // TT_ERROR_H_
