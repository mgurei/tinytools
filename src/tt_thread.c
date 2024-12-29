/**
 * @file tt_thread.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_thread.h"
#include "tt_atomic.h"
#include "tt_types.h"
#include <bits/pthreadtypes.h>
#include <stdlib.h>
#include <string.h>

/* Platform-specific includes */
#if defined(__linux__)
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#define TT_PLATFORM_LINUX

#elif defined(ARDUINO)
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#define TT_PLATFORM_ARDUINO

#elif defined(FREE_RTOS)
#include "FreeRTOS.h"
#include "task.h"
#define TT_PLATFORM_FREERTOS

#else
/* Bare metal - implement minimal threading */
#define TT_PLATFORM_BARE
#endif

struct tt_thread_t {
#if defined(TT_PLATFORM_LINUX)
  pthread_t handle;

#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
  TaskHandle_t handle;
  StaticTask_t *task_buffer;
  StackType_t *stack_buffer;

#else
  /* Bare metal implementation */
  void *stack_ptr;
  size_t stack_size;
  volatile uint32_t *context;
#endif

  tt_thread_func_t func;
  void *arg;
  tt_atomic_int_t state;
  tt_thread_attr_t attrs;
  void *retval;
};

/* Default stack size for different platforms */
#if defined(TT_PLATFORM_ARDUINO)
#define TT_DEFAULT_STACK_SIZE (configMININAL_STACK_SIZE * 2)

#elif defined(TT_PLATFORM_FREERTOS)
#define TT_DEFAULT_STACK_SIZE (configMINIMAL_STACK_SIZE * 4)

#else
#define TT_DEFAULT_STACK_SIZE (4096) /* Default for Linux and bare metal */

#endif

static const tt_thread_attr_t default_attrs = {
    .priority = TT_THREAD_PRIORITY_NORMAL,
    .stack_size = TT_DEFAULT_STACK_SIZE,
    .name = "tt_thread"};

tt_error_t tt_thread_attr_init(tt_thread_attr_t *attr) {
  if (attr == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  *attr = default_attrs;
  return TT_SUCCESS;
}

#if defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
static void thread_start_routine(void *arg) {
  tt_thread_t *thread = (tt_thread_t *)arg;
  if (thread == NULL) {
    vTaskDelete(NULL);
    return;
  }

  tt_atomic_store(&thread->state, TT_THREAD_STATE_RUNNING,
                  TT_MEMORY_ORDER_RELEASE);
  thread->retval = thread->func(thread->arg);
  tt_atomic_store(&thread->state, TT_THREAD_STATE_TERMINATED,
                  TT_MEMORY_ORDER_RELEASE);

  vTaskDelete(NULL);
}

#elif defined(TT_PLATFORM_LINUX)
static void *thread_start_routine(void *arg) {
  tt_thread_t *thread = (tt_thread_t *)arg;
  if (thread == NULL) {
    return NULL;
  }

  tt_atomic_store(&thread->state, TT_THREAD_STATE_RUNNING,
                  TT_MEMORY_ORDER_RELEASE);
  thread->retval = thread->func(thread->arg);
  tt_atomic_store(&thread->state, TT_THREAD_STATE_TERMINATED,
                  TT_MEMORY_ORDER_RELEASE);

  return NULL;
}
#endif

tt_error_t tt_thread_create(tt_thread_t **thread, const tt_thread_attr_t *attr,
                            tt_thread_func_t func, void *arg) {
  if (thread == NULL || func == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  tt_thread_t *new_thread = (tt_thread_t *)malloc(sizeof(tt_thread_t));
  if (new_thread == NULL) {
    return TT_ERROR_MEMORY;
  }

  new_thread->func = func;
  new_thread->arg = arg;
  new_thread->attrs = attr ? *attr : default_attrs;
  tt_atomic_init(&new_thread, TT_THREAD_STATE_CREATED);

#if defined(TT_PLATFORM_LINUX)
  pthread_attr_t pthread_attr;
  pthread_attr_init(&pthread_attr);

  if (new_thread->attrs.stack_size > 0) {
    pthread_attr_setstacksize(&pthread_attr, new_thread->attrs.stack_size);
  }

  int result = pthread_create(&new_thread->handle, &pthread_attr,
                              thread_start_routine, new_thread);

  pthread_attr_destroy(&pthread_attr);

  if (result != 0) {
    free(new_thread);
    return TT_ERROR_THREAD_CREATE;
  }
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
  new_thread->stack_buffer =
      (StackType_t *)malloc(new_thread->attrs.stack_size * sizeof(StackType_t));
  new_thread->task_buffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));

  if (new_thread->stack_buffer == NULL || new_thread->task_buffer == NULL) {
    free(new_thread->stack_buffer);
    free(new_thread->task_buffer);
    free(new_thread);
    return TT_ERROR_MEMORY;
  }

  new_thread->handle = xTaskCreateStatic(
      thread_start_routine, new_thread->attrs.name,
      new_thread->attrs.stack_size, new_thread, new_thread->attrs.priority,
      new_thread->stack_buffer, new_thread->task_buffer);

  if (new_thread->handle == NULL) {
    free(new_thread->stack_buffer);
    free(new_thread->task_buffer);
    free(new_thread);
    return TT_ERROR_THREAD_CREATE;
  }
#else
  return TT_ERROR_NOT_IMPLEMENTED;
#endif

  *thread = new_thread;
  return TT_SUCCESS;
}

tt_error_t tt_thread_join(tt_thread_t *thread, void **retval) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

#if defined(TT_PLATFORM_LINUX)
  int result = pthread_join(thread->handle, retval);
  return (result == 0) ? TT_SUCCESS : TT_ERROR_THREAD_JOIN;
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
  while (tt_atomic_load(&thread->state, TT_MEMORY_ORDER_ACQUIRE) !=
         TT_THREAD_STATE_TERMINATED) {
    vTaskDelay(1);
  }
  if (retval) {
    *retval = thread->retval;
  }
  return TT_SUCCESS;
#else
  return TT_ERROR_NOT_IMPLEMENTED;
#endif
}

tt_error_t tt_thread_get_state(const tt_thread_t *thread,
                               tt_thread_state_t *state) {
  if (thread == NULL || state = NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  *state = tt_atomic_load(&thread->state, TT_MEMORY_ORDER_ACQUIRE);
  return TT_SUCCESS;
}

tt_error_t tt_thread_set_priority(tt_thread_t *thread,
                                  tt_thread_priority_t priority) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

#if defined(TT_PLATFORM_LINUX)
  struct sched_param param;
  int policy;
  pthread_getschedparam(thread->handle, &policy, &param);
  param.sched_priority = priority;
  int result = pthread_setschedparam(thread->handle, policy, &param);
  return (result == 0) ? TT_SUCCESS : TT_ERROR_THREAD_PRIORITY;
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
  vTaskPrioritySet(thread->handle, priority);
  return TT_SUCCESS;
#else
  return TT_ERROR_NOT_IMPLEMENTED;
#endif
}

tt_error_t tt_thread_suspend(tt_thread_t *thread) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

#if defined(TT_PLATFORM_LINUX)
  // Linux doesn't have direct thread suspension
  // TODO: See what to do about this
  return TT_ERROR_NOT_IMPLEMENTED;
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
  vTaskSuspend(thread_handle->handle);
  tt_atomic_store(&thread->state, TT_THREAD_STATE_SUSPEND,
                  TT_MEMORY_ORDER_RELEASE);
  return TT_SUCCESS;
#else
  return TT_ERROR_NOT_IMPLEMENTED;
#endif
}

tt_error_t tt_thread_resume(tt_thread_t *thread) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

#if defined(TT_PLATFORM_LINUX)
  // Linux doesn't have direct thread suspension
  // TODO: See what to do about this
  return TT_ERROR_NOT_IMPLEMENTED;
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
  vTaskResume(thread_handle->handle);
  tt_atomic_store(&thread->state, TT_THREAD_STATE_RUNNING,
                  TT_MEMORY_ORDER_RELEASE);
  return TT_SUCCESS;
#else
  return TT_ERROR_NOT_IMPLEMENTED;
#endif
}

tt_error_t tt_thread_sleep(uint32_t ms) {
#if defined(TT_PLATFORM_LINUX)
  struct timespecs ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) / 1000000;
  return (nanosleep(&ts, NULL) == 0) ? TT_SUCCESS : TT_ERROR_THREAD_SLEEP;
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS) #else
  vTaskDelay(pdMS_TO_TICKS(ms));
  return TT_SUCCESS;
#else
  return TT_ERROR_NOT_IMPLEMENTED;
#endif
}

tt_thread_t *tt_thread_self(void) {
#if defined(TT_PLATFORM_LINUX)
  pthread_t self = pthread_self();
  // NOTE: This needs a thread lookup table implementation
  return NULL; // TODO: Temporary until lookup table is implemented
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
  TaskHandle_t current = xTaskGetCurrentTaskHandle();
  // NOTE: This needs a thread lookup table implementation
  return NULL; // TODO: Temporary until lookup table is implemented
#else
  return NULL;
#endif
}

tt_error_t tt_thread_destroy(tt_thread_t *thread) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

#if defined(TT_PLATFORM_LINUX)

#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
  vTaskDelete(thread->handle);
  free(thread->stack_buffer);
  free(thread->task_buffer);
#else
#endif

  free(thread);
  return TT_SUCCESS;
}
