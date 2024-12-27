/**
 * @file tt_error.c
 * @brief Error handling implementation
 * @author mgurei
 * @date 2024-12-27
 */

#include "tt_error.h"

const char *tt_error_to_string(tt_error_t error) {
  switch (error) {
  case TT_SUCCESS:
    return "Success";
  case TT_ERROR_INVALID_PARAM:
    return "Invalid parameter";
  case TT_ERROR_NULL_POINTER:
    return "Null pointer";
  case TT_ERROR_BUFFER_FULL:
    return "Buffer full";
  case TT_ERROR_BUFFER_EMPTY:
    return "Buffer empty";
  case TT_ERROR_NOT_INITIALIZED:
    return "Not initialized";
  case TT_ERROR_PLATFORM_SPECIFIC:
    return "Platform specific error";
  default:
    return "Unknown error";
  }
}
