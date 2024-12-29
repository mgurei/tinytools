/**
 * @file tt_atomic.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-28
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_ATOMIC_H_
#define TT_ATOMIC_H_

#include "tt_types.h"

/**
 * @brief Memory ordering for atomic operations
 */
typedef enum {
  TT_MEMORY_ORDER_RELAXED = 0, /**< No synchronization/ordering constraints*/
  TT_MEMORY_ORDER_ACQUIRE,     /**< Ensure this CPU reads happen after*/
  TT_MEMORY_ORDER_RELEASE,     /**< Ensure this CPU writes happen before*/
  TT_MEMORY_ORDER_ACQ_REL,     /**< Combination of acquire and release*/
  TT_MEMORY_ORDER_SEQ_CST,     /**< Sequential consistency (full fence)*/
} tt_memory_order_t;

/**
 * @brief Atomic integer type
 */
typedef struct {
  volatile int32_t value; /**< The atomic value*/
} tt_atomic_int_t;

/**
 * @brief Initialize atomic integer
 * @param atomic Pointer to atomic integer
 * @param initial_value Initial value
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_atomic_init(tt_atomic_int_t *atomic, int32_t initial_value);

/**
 * @brief Load value from atomic integer
 *
 * Detailed description
 *
 * @param atomic Pointer to atomic integer
 * @param order Memory ordering constraint
 * @return The loaded value
 */
int32_t tt_atomic_load(const tt_atomic_int_t *atomic, tt_memory_order_t order);

/**
 * @brief Store value to atomic integer
 * @param atomic Pointer to atomic integer
 * @param value Value to add
 * @param order Memory ordering constraint
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_atomic_store(tt_atomic_int_t *atomic, int32_t value,
                           tt_memory_order_t order);

/**
 * @brief Atomic add opetation
 * @param atomic Pointer to atomic integer
 * @param value Value to add
 * @param order Memory ordering constraint
 * @return Previous value
 */
int32_t tt_atomic_add(tt_atomic_int_t *atomic, int32_t value,
                      tt_memory_order_t order);

/**
 * @brief Atomic subtract opetation
 * @param atomic Pointer to atomic integer
 * @param value Value to subtract
 * @param order Memory ordering constraint
 * @return Previous value
 */
int32_t tt_atomic_sub(tt_atomic_int_t *atomic, int32_t value,
                      tt_memory_order_t order);

/**
 * @brief Compare and exchange operation
 * @param atomic Pointer to atomic integer
 * @param expected Expected value
 * @param desired Desired value
 * @param order Memory ordering constraint
 * @return true if exchange occured, false otherwise
 */
bool tt_atomic_compare_exchange(tt_atomic_int_t *atomic, int32_t *expected,
                                int32_t desired, tt_memory_order_t order);

/**
 * @brief Memory fence operation
 * @param order Memory ordering constraint
 */
void tt_atomic_thread_fence(tt_memory_order_t order);

#endif // TT_ATOMIC_H_
