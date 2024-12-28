/**
 * @file tt_error.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-27
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_ERROR_H_
#define TT_ERROR_H_

#include "tt_mutex.h"
#include "tt_types.h"

/**
 * @brief Error context structure
 */
typedef struct {
  tt_error_t last_error;        /**< Last error occured*/
  void (*callback)(tt_error_t); /**< Error callback function*/
  tt_mutex_t mutex;             /**< Mutex for thread safety*/
  bool initialized;             /**< Initialization state*/
} tt_error_context_t;

/**
 * @brief Initialize the error handling system
 * @return TT_SUCCESS if initialized successfully
 */
tt_error_t tt_error_init(void);

/**
 * @brief Deinitialize the error handling system
 * @return TT_SUCCESS if deinitialized successfully, error code otherwise
 */
tt_error_t tt_error_deinit(void);

/**
 * @brief Get the last error that occured
 * @return The last error code
 */
tt_error_t tt_error_get_last(void);

/**
 * @brief Set the last error code
 * @param error Error code to convert
 * @return TT_SUCCESS if set successfully, error code otherwise
 */
tt_error_t tt_error_set_last(tt_error_t error);

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
 * @return TT_SUCCESS if cleared successfully, error code otherwise
 */
tt_error_t tt_error_clear(void);

/**
 * @brief Register a callback for error handling
 * @param callback Function pointer to error handle
 * @return TT_SUCCESS if callback was register successfully, error code
 * otherwise
 */
tt_error_t tt_error_register_callback(void (*callback)(tt_error_t));

#endif // TT_ERROR_H_
