/**
 * @file tt_error.c
 * @brief Error handling implementation
 * @author mgurei
 * @date 2024-12-27
 */

#include "tt_error.h"
#include <stddef.h>

static tt_error_context_t error_ctx = {
    .last_error = TT_SUCCESS, .callback = NULL, .initialized = false};

tt_error_t tt_error_init(void) {
  if (error_ctx.initialized) {
    return TT_SUCCESS;
  }

  tt_error_t result = tt_mutex_init(&error_ctx.mutex);
  if (result != TT_SUCCESS) {
    return result;
  }

  error_ctx.last_error = TT_SUCCESS;
  error_ctx.callback = NULL;
  error_ctx.initialized = true;

  return TT_SUCCESS;
}

tt_error_t tt_error_deinit(void) {
  if (!error_ctx.initialized) {
    return TT_ERROR_NOT_INITIALIZED;
  }

  tt_error_t result = tt_mutex_lock(&error_ctx.mutex);
  if (result != TT_SUCCESS) {
    return result;
  }

  error_ctx.callback = NULL;
  error_ctx.initialized = false;

  result = tt_mutex_unlock(&error_ctx.mutex);
  if (result != TT_SUCCESS) {
    return result;
  }

  return TT_SUCCESS;
}

tt_error_t tt_error_get_last(void) {
  if (!error_ctx.initialized) {
    return TT_ERROR_NOT_INITIALIZED;
  }

  tt_error_t error;
  tt_error_t result = tt_mutex_lock(&error_ctx.mutex);
  if (result != TT_SUCCESS) {
    return result;
  }

  error = error_ctx.last_error;

  result = tt_mutex_unlock(&error_ctx.mutex);
  if (result != TT_SUCCESS) {
    return result;
  }

  return error;
}

tt_error_t tt_error_set_last(tt_error_t error) {
  if (!error_ctx.initialized) {
    return TT_ERROR_NOT_INITIALIZED;
  }

  tt_error_t result = tt_mutex_lock(&error_ctx.mutex);
  if (result != TT_SUCCESS) {
    return result;
  }

  error_ctx.last_error = error;
  void (*callback)(tt_error_t) = error_ctx.callback;

  result = tt_mutex_unlock(&error_ctx.mutex);
  if (result != TT_SUCCESS) {
    return result;
  }

  if (callback != NULL && error != TT_SUCCESS) {
    callback(error);
  }

  return TT_SUCCESS;
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
  case TT_ERROR_MUTEX_ERROR:
    return "Mutex operation failed";
  default:
    return "Unknown error";
  }
}

bool tt_error_is_success(void) { return tt_error_get_last() == TT_SUCCESS; }

tt_error_t tt_error_clear(void) { return tt_error_set_last(TT_SUCCESS); }

tt_error_t tt_error_register_callback(void (*callback)(tt_error_t)) {
  if (!error_ctx.initialized) {
    return TT_ERROR_NOT_INITIALIZED;
  }

  tt_error_t result = tt_mutex_lock(&error_ctx.mutex);
  if (result != TT_SUCCESS) {
    return result;
  }

  error_ctx.callback = callback;

  result = tt_mutex_unlock(&error_ctx.mutex);
  if (result != TT_SUCCESS) {
    return result;
  }

  return TT_SUCCESS;
}
