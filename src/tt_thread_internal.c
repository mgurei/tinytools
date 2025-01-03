/**
 * @file tt_thread_internal.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_thread_internal.h"
#include "tt_mutex.h"
#include "tt_types.h"
#include <string.h>

#if defined(TT_CAP_THREADS)
tt_thread_table_entry_t g_thread_table[TT_MAX_THREADS] = {0};
tt_mutex_t g_thread_table_mutex = {0};

tt_error_t tt_thread_table_init(void) {
  tt_error_t result = tt_mutex_init(&g_thread_table_mutex);
  if (result != TT_SUCCESS) {
    return result;
  }

  tt_mutex_lock(&g_thread_table_mutex);
  memset(g_thread_table, 0, sizeof(g_thread_table));
  tt_mutex_unlock(&g_thread_table_mutex);
  return TT_SUCCESS;
}

tt_error_t tt_thread_table_cleanup(void) {
  tt_mutex_lock(&g_thread_table_mutex);
  memset(g_thread_table, 0, sizeof(g_thread_table));
  tt_mutex_unlock(&g_thread_table_mutex);
  return TT_SUCCESS;
}

tt_error_t tt_thread_table_register(tt_thread_t *thread) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  tt_error_t result = TT_ERROR_NOT_FOUND;

  tt_mutex_lock(&g_thread_table_mutex);

  // Find first available slot
  for (size_t i = 0; i < TT_MAX_THREADS; i++) {
    if (!g_thread_table[i].in_use) {
      g_thread_table[i].thread = thread;
      g_thread_table[i].in_use = true;
      result = TT_SUCCESS;
      break;
    }
  }

  tt_mutex_unlock(&g_thread_table_mutex);
  return result;
}

tt_error_t tt_thread_table_unregister(tt_thread_t *thread) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  tt_error_t result = TT_ERROR_NOT_FOUND;

  tt_mutex_lock(&g_thread_table_mutex);

  // Find first available slot
  for (size_t i = 0; i < TT_MAX_THREADS; i++) {

    if (g_thread_table[i].in_use && g_thread_table[i].thread == thread) {
      g_thread_table[i].thread = NULL;
      g_thread_table[i].in_use = false;
      result = TT_SUCCESS;
      break;
    }
  }

  tt_mutex_unlock(&g_thread_table_mutex);
  return result;
}

tt_thread_t *
tt_thread_table_find_by_handle(tt_platform_thread_handle_t handle) {
  tt_thread_t *result = NULL;

  tt_mutex_lock(&g_thread_table_mutex);

  for (size_t i = 0; i < TT_MAX_THREADS; i++) {
    if (g_thread_table[i].in_use &&
        memcmp(&g_thread_table[i].thread->handle, &handle,
               sizeof(tt_platform_thread_handle_t))) {
      result = g_thread_table[i].thread;
      break;
    }
  }

  tt_mutex_unlock(&g_thread_table_mutex);
  return result;
}

#endif /* TT_CAP_THREADS */
