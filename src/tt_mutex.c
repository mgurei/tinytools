/**
 * @file tt_mutex.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-28
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_mutex.h"
#include "tt_platform.h"
#include <stddef.h>

#if defined(TT_TARGET_LINUX) && defined(TT_CAP_MUTEX)
tt_error_t tt_mutex_init(tt_mutex_t *mutex) {
  return tt_platform_mutex_init(mutex);
}
tt_error_t tt_mutex_destroy(tt_mutex_t *mutex) {
  return tt_platform_mutex_destroy(mutex);
}
tt_error_t tt_mutex_lock(tt_mutex_t *mutex) {
  return tt_platform_mutex_lock(mutex);
}
tt_error_t tt_mutex_unlock(tt_mutex_t *mutex) {
  return tt_platform_mutex_unlock(mutex);
}
tt_error_t tt_mutex_trylock(tt_mutex_t *mutex) {
  return tt_platform_mutex_trylock(mutex);
}
bool tt_mutex_is_locked(tt_mutex_t *mutex) {
  return tt_platform_mutex_is_locked(mutex);
}
#else /* General non-platorm specific mutex implementation */

tt_error_t tt_mutex_init(tt_mutex_t *mutex) {
  if (mutex == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  mutex->lock = TT_MUTEX_UNLOCKED;
  mutex->initialized = true;
  return TT_SUCCESS;
}

tt_error_t tt_mutex_destroy(tt_mutex_t *mutex) {
  if (mutex == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  if (!mutex->initialized) {
    return TT_ERROR_NOT_INITIALIZED;
  }

  mutex->initialized = false;
  return TT_SUCCESS;
}

tt_error_t tt_mutex_lock(tt_mutex_t *mutex) {
  if (mutex == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  if (!mutex->initialized) {
    return TT_ERROR_NOT_INITIALIZED;
  }

  /* Basic spinlock implementation */
  while (__atomic_test_and_set(&mutex->lock, __ATOMIC_SEQ_CST)) {
    /* Wait until lock is available */
  }

  return TT_SUCCESS;
}

tt_error_t tt_mutex_unlock(tt_mutex_t *mutex) {
  if (mutex == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  if (!mutex->initialized) {
    return TT_ERROR_NOT_INITIALIZED;
  }

  __atomic_clear(&mutex->lock, __ATOMIC_SEQ_CST);
  return TT_SUCCESS;
}

tt_error_t tt_mutex_trylock(tt_mutex_t *mutex) {
  if (mutex == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  if (!mutex->initialized) {
    return TT_ERROR_NOT_INITIALIZED;
  }

  if (__atomic_test_and_set(&mutex->lock, __ATOMIC_SEQ_CST)) {
    return TT_ERROR_BUSY;
  }

  return TT_SUCCESS;
}

bool tt_mutex_is_locked(tt_mutex_t *mutex) {
  if (mutex == NULL || !mutex->initialized) {
    return false;
  }

  if (__atomic_load_n(&mutex->lock, __ATOMIC_SEQ_CST)) {
    return true;
  }

  return false;
}

#endif /* TT_TARGET_LINUX && TT_CAP_MUTEX */
