/**
 * @file tt_error.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-27
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_ERROR_H_
#define TT_ERROR_H_

#include <stdint.h>

typedef enum {
  TT_SUCCESS = 0,
  TT_ERROR_INVALID_PARAM,
  TT_ERROR_NULL_POINTER,
  TT_ERROR_BUFFER_FULL,
  TT_ERROR_BUFFER_EMPTY,
  TT_ERROR_NOT_INITIALIZED,
  TT_ERROR_PLATFORM_SPECIFIC,
  // Add more error codes as needed
} tt_error_t;

/**
 * @brief Convert error code to string
 * @param error Error code
 * @return String representation of error
 */
const char *tt_error_to_string(tt_error_t error);

#endif // TT_ERROR_H_
