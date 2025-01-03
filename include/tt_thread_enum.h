/**
 * @file tt_thread_enum.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2025-01-03
 * @brief
 * @copyright Copyright (c) 2025 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_THREAD_ENUM_H_
#define TT_THREAD_ENUM_H_

#if defined(TT_CAP_THREADS)
/**
 * @brief Thread priority levels
 */
typedef enum {
  TT_THREAD_PRIORITY_LOW = 0,
  TT_THREAD_PRIORITY_NORMAL,
  TT_THREAD_PRIORITY_HIGH,
  TT_THREAD_PRIORITY_REALTIME
} tt_thread_priority_t;

/**
 * @brief Thread state
 */
typedef enum {
  TT_THREAD_STATE_CREATED = 0,
  TT_THREAD_STATE_RUNNING,
  TT_THREAD_STATE_SUSPENDED,
  TT_THREAD_STATE_TERMINATED
} tt_thread_state_t;
#endif /* TT_CAP_THREADS */

#endif // TT_THREAD_ENUM_H_
