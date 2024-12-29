/**
 * @file tt_atomic.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_atomic.h"
#include "tt_types.h"
#include <stddef.h>

#if defined(__GNUC__) || defined(__clang__)
/* GCC/Clang atomic built-ins */
#define ATOMIC_LOAD(ptr, order) __atomic_load_n(ptr, order)
#define ATOMIC_STORE(ptr, val, order) __atomic_store_n(ptr, val, order)
#define ATOMIC_ADD(ptr, val, order) __atomic_fetch_add(ptr, val, order)
#define ATOMIC_SUB(ptr, val, order) __atomic_fetch_sub(ptr, val, order)
#define ATOMIC_CAS(ptr, expected, desired, order)                              \
  __atomic_compare_exchange_n(ptr, expected, desired, false, order, order)
#else
#error "Unsupported compiler for atomic operations"
#endif

tt_error_t tt_atomic_init(tt_atomic_int_t *atomic, int32_t value) {
  if (atomic == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  atomic->value = value;
  return TT_SUCCESS;
}

int32_t tt_atomic_load(tt_atomic_int_t *atomic, tt_memory_order_t order) {
  if (atomic == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  return ATOMIC_LOAD(&atomic->value, order);
}

tt_error_t tt_atomic_store(tt_atomic_int_t *atomic, int32_t value,
                           tt_memory_order_t order) {
  if (atomic == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  ATOMIC_STORE(&atomic->value, value, order);
  return TT_SUCCESS;
}

int32_t tt_atomic_add(tt_atomic_int_t *atomic, int32_t value,
                      tt_memory_order_t order) {
  if (atomic == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  return ATOMIC_ADD(&atomic->value, value, order);
}

int32_t tt_atomic_sub(tt_atomic_int_t *atomic, int32_t value,
                      tt_memory_order_t order) {

  if (atomic == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  return ATOMIC_SUB(&atomic->value, value, order);
}

bool tt_atomic_compare_exchange(tt_atomic_int_t *atomic, int32_t *expected,
                                int32_t desired, tt_memory_order_t order) {
  if (atomic == NULL || expected == NULL) {
    return false;
  }

  return ATOMIC_CAS(&atomic->value, expected, desired, order);
}
