/**
 * @file tt_error.c
 * @brief Error handling implementation
 * @author mgurei
 * @date 2024-12-27
 */

#include "tt_error.h"
#include <stddef.h>

#ifndef TT_USE_THREADING
#include "tt_mutex.h"
static tt_mutex_t error_mutex;
#endif

static tt_error_t last_error = TT_SUCCESS;
static void (*error_callback)(tt_error_t) = NULL;

tt_error_t tt_error_init(void) {
#ifdef TT_USE_THREADING
  tt_error_t result = tt_mutex_init(&error_mutex);
  if (result != TT_SUCCESS) {
    return result;
  }
#endif
  last_error = TT_SUCCESS;
  error_callback = NULL;
  return TT_SUCCESS;
}

tt_error_t tt_error_get_last(void) {
  tt_error_t error;
#ifdef TT_USE_THREADING
  tt_mutex_lock(&error_mutex);
#endif
  error = last_error;
#ifdef TT_USE_THREADING
  tt_mutex_unlock(&error_mutex);
#endif
  return error;
}

void tt_error_set_last(tt_error_t error) {
#ifdef TT_USE_THREADING
  tt_mutex_lock(&error_mutex);
#endif
  last_error = error;
  if (error_callback != NULL && error != TT_SUCCESS) {
    error_callback(error);
  }
#ifdef TT_USE_THREADING
  tt_mutex_unlock(&error_mutex);
#endif
}

const char *tt_error_to_string(tt_error_t error) {
  switch (error) {
  case TT_SUCCESS:
    return "Success";
  case TT_ERROR_NULL_POINTER:
    return "Null pointer";
  case TT_ERROR_INVALID_PARAM:
    return "Invalid parameter";
  case TT_ERROR_BUFFER_FULL:
    return "Buffer full";
  case TT_ERROR_BUFFER_EMPTY:
    return "Buffer empty";
  case TT_ERROR_OUT_OF_MEMORY:
    return "Memory allocation failed";
  case TT_ERROR_TIMEOUT:
    return "Operation timed out";
  case TT_ERROR_BUSY:
    return "Resource is busy";
  case TT_ERROR_NOT_INITIALIZED:
    return "Not initialized";
  case TT_ERROR_PLATFORM_SPECIFIC:
    return "Platform specific error";
  default:
    return "Unknown error";
  }
}

bool tt_error_is_success(void) { return tt_error_get_last() == TT_SUCCESS; }

void tt_error_clear(void) { tt_error_set_last(TT_SUCCESS); }

tt_error_t tt_error_register_callback(void (*callback)(tt_error_t)) {
  if (callback == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

#ifdef TT_USE_THREADING
  tt_mutex_lock(&error_mutex);
#endif
  error_callback = callback;
#ifdef TT_USE_THREADING
  tt_mutex_unlock(&error_mutex);
#endif
  return TT_SUCCESS;
}
