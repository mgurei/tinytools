/**
 * @file tt_thread.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-29
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_thread.h"
#include "tt_types.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Default stack size for different platforms */
#if defined(TT_PLATFORM_ARDUINO)
#define TT_DEFAULT_STACK_SIZE (configMININAL_STACK_SIZE * 2)

#elif defined(TT_PLATFORM_FREERTOS)
#define TT_DEFAULT_STACK_SIZE (configMINIMAL_STACK_SIZE * 4)

#else
#define TT_DEFAULT_STACK_SIZE                                                  \
  (8 * 1024 * 1024) /* Default for Linux and bare metal */

#endif

#if defined(TT_CAP_THREAD) && defined(TT_TARGET_LINUX)

static const tt_thread_attr_t default_attrs = {
    .priority = TT_THREAD_PRIORITY_NORMAL,
    .stack_size = (8 * 1024 * 1024), // Default for Linux and bare metal
    .name = "tt_thread"};

tt_error_t tt_thread_init(void) { return tt_thread_table_init(); }

tt_error_t tt_thread_attr_init(tt_thread_attr_t *attr) {
  if (attr == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  static const tt_thread_attr_t default_attrs = {
      .priority = TT_THREAD_PRIORITY_NORMAL,
      .stack_size = TT_DEFAULT_STACK_SIZE,
      .name = "tt_thread"};
  *attr = default_attrs;
  return TT_SUCCESS;
}

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
  new_thread->state = TT_THREAD_STATE_CREATED;
  new_thread->is_active = true;
  new_thread->stack_size = attr ? attr->stack_size : TT_DEFAULT_STACK_SIZE;
  new_thread->priority = attr ? attr->priority : TT_THREAD_PRIORITY_NORMAL;
  new_thread->name = attr ? attr->name : "tt_thread";

  tt_error_t result = tt_platform_thread_create(new_thread, attr, func, arg);
  if (result != TT_SUCCESS) {
    free(new_thread);
    return result;
  }

  result = tt_thread_table_register(new_thread);
  if (result != TT_SUCCESS) {
    tt_platform_thread_destroy(new_thread);
    free(new_thread);
    return result;
  }
  *thread = new_thread;
  return TT_SUCCESS;
}

tt_error_t tt_thread_join(tt_thread_t *thread, void **retval) {
  return tt_platform_thread_join(thread, retval);
}

tt_error_t tt_thread_get_state(const tt_thread_t *thread,
                               tt_thread_state_t *state) {
  if (thread == NULL || state == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  *state = thread->state;
  return TT_SUCCESS;
}

tt_error_t tt_thread_set_priority(tt_thread_t *thread,
                                  tt_thread_priority_t priority) {
  return tt_platform_thread_set_priority(thread, priority);
}

tt_error_t tt_thread_suspend(tt_thread_t *thread) {
  return tt_platform_thread_suspend(thread);
}

tt_error_t tt_thread_resume(tt_thread_t *thread) {
  return tt_platform_thread_resume(thread);
}

tt_error_t tt_thread_spleep(uint32_t ms) {
  return tt_platform_thread_sleep(ms);
}

tt_thread_t *tt_thread_self(void) { return tt_platform_thread_self(); }

tt_error_t tt_thread_destroy(tt_thread_t *thread) {
  return tt_platform_thread_destroy(thread);
}

#elif (TT_TARGET_FREERTOS) // TODO: organize code below per platform!

/* Platform-specific includes */
#if defined(TT_TARGET_LINUX)
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#elif defined(TT_TARGET_ARDUINO)
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#define TT_PLATFORM_ARDUINO

#elif defined(TT_TARGET_FREERTOS)
#include "FreeRTOS.h"
#include "task.h"
#define TT_PLATFORM_FREERTOS

#else
/* Bare metal - implement minimal threading */
#define TT_PLATFORM_BARE
#endif

static const tt_thread_attr_t default_attrs = {
    .priority = TT_THREAD_PRIORITY_NORMAL,
    .stack_size = TT_DEFAULT_STACK_SIZE,
    .name = "tt_thread"};

tt_error_t tt_thread_init(void) { return tt_thread_table_init(); }

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
    v

        thread->state = TT_THREAD_STATE_RUNNING;
    thread->retval = thread->func(thread->arg);
    thread->state = TT_THREAD_STATE_TERMINATED;

    return thread->retval;
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
  new_thread->state = TT_THREAD_STATE_CREATED;
  new_thread->is_active = true;
  new_thread->stack_size = attr ? attr->stack_size : default_attrs.stack_size;
  new_thread->priority = attr ? attr->priority : default_attrs.priority;
  new_thread->name = attr ? attr->name : default_attrs.name;

#if defined(TT_PLATFORM_LINUX)
  pthread_attr_t pthread_attr;
  pthread_attr_init(&pthread_attr);

  if (new_thread->stack_size > 0) {
    pthread_attr_setstacksize(&pthread_attr, new_thread->stack_size);
  }

  pthread_t *handle = (pthread_t *)malloc(sizeof(pthread_t));
  if (handle == NULL) {
    free(new_thread);
    pthread_attr_destroy(&pthread_attr);
    return TT_ERROR_MEMORY;
  }

  int result =
      pthread_create(handle, &pthread_attr, thread_start_routine, new_thread);
  pthread_attr_destroy(&pthread_attr);

  if (result != 0) {
    free(handle);
    free(new_thread);
    return TT_ERROR_THREAD_CREATE;
  }

  new_thread->handle = (tt_platform_thread_handle_t)handle;

#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
    TaskHandle_t task_handle = NULL;
    BaseType_t result = xTaskCreate(
        thread_start_routine, attr ? attr->name : "tt_thread",
        attr ? attr->stack_size : TT_DEFAULT_STACK_SIZE, new_thread,
        attr ? attr->priority : TT_THREAD_PRIORITY_NORMALE, &task_handle);

    if (result != pdPASS || task_handle == NULL) {
      free(new_thread);
      return TT_ERROR_THREAD_CREATE;
    }

    new_thread->handle = (platform_thread_handle_t)task_handle;
#else
  return TT_ERROR_NOT_IMPLEMENTED;
#endif

  // Register the thread in the thread table
  tt_error_t reg_result = tt_thread_table_register(new_thread);
  if (reg_result != TT_SUCCESS) {
#if defined(TT_PLATFORM_LINUX)
    pthread_t *pthread_handle = (pthread_t *)new_thread->handle;
    pthread_detach(*pthread_handle);
    free(pthread_handle);
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
      vTaskDelete((TaskHandle_t)new_thread->handle);
#endif
    free(new_thread);
    return reg_result;
  }

  *thread = new_thread;
  return TT_SUCCESS;
}

tt_error_t tt_thread_join(tt_thread_t *thread, void **retval) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

#if defined(TT_PLATFORM_LINUX)
  pthread_t *pthread_handle = (pthread_t *)thread->handle;
  int result = pthread_join(*pthread_handle, retval);
  return (result == 0) ? TT_SUCCESS : TT_ERROR_THREAD_JOIN;
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
    while (thread->state != TT_THREAD_STATE_TERMINATED) {
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
  if (thread == NULL || state == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  *state = thread->state;
  return TT_SUCCESS;
}

tt_error_t tt_thread_set_priority(tt_thread_t *thread,
                                  tt_thread_priority_t priority) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

#if defined(TT_PLATFORM_LINUX)
  struct sched_param param;
  int policy = SCHED_OTHER; // Use the default scheduler
  int min_prio = sched_get_priority_min(policy);
  int max_prio = sched_get_priority_max(policy);

  // Map our priority levels to the system's priority range
  int system_priority;
  switch (priority) {
  case TT_THREAD_PRIORITY_LOW:
    system_priority = min_prio;
    break;
  case TT_THREAD_PRIORITY_NORMAL:
    system_priority = min_prio + (max_prio - min_prio) / 3;
    break;
  case TT_THREAD_PRIORITY_HIGH:
    system_priority = min_prio + 2 * (max_prio - min_prio) / 3;
    break;
  case TT_THREAD_PRIORITY_REALTIME:
    system_priority = max_prio;
    break;
  default:
    return TT_ERROR_INVALID_PARAM;
  }

  param.sched_priority = system_priority;
  pthread_t *pthread_handle = (pthread_t *)thread->handle;
  pthread_setschedparam(*pthread_handle, policy, &param);
  thread->priority = priority;
  return TT_SUCCESS;

#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
    vTaskPrioritySet(thread->handle, priority);
    thread->priority = priority;
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
    thread->state = TT_THREAD_STATE_SUSPENDED;
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
    thread->state = TT_THREAD_STATE_RUNNING;
    return TT_SUCCESS;
#else
  return TT_ERROR_NOT_IMPLEMENTED;
#endif
}

tt_error_t tt_thread_sleep(uint32_t ms) {
#if defined(TT_PLATFORM_LINUX)
  struct timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) / 1000000;
  return (nanosleep(&ts, NULL) == 0) ? TT_SUCCESS : TT_ERROR_THREAD_SLEEP;
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
    vTaskDelay(pdMS_TO_TICKS(ms));
    return TT_SUCCESS;
#else
  return TT_ERROR_NOT_IMPLEMENTED;
#endif
}

tt_thread_t *tt_thread_self(void) {
#if defined(TT_PLATFORM_LINUX)
  pthread_t handle = pthread_self();
  return tt_thread_table_find_by_handle(handle);
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    return tt_thread_table_find_by_handle(handle);
#else
  return NULL;
#endif
}

tt_error_t tt_thread_destroy(tt_thread_t *thread) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  // First, ensure thread is not running
  if (thread->state != TT_THREAD_STATE_TERMINATED) {
    return TT_ERROR_THREAD_ACTIVE;
  }

  // Unregister from thread table
  tt_error_t result = tt_thread_table_unregister(thread);
  if (result != TT_SUCCESS) {
    return result;
  }

#if defined(TT_PLATFORM_LINUX)
  pthread_t *pthread_handle = (pthread_t *)thread->handle;
  pthread_detach(*pthread_handle);
  free(pthread_handle);
#elif defined(TT_PLATFORM_ARDUINO) || defined(TT_PLATFORM_FREERTOS)
    vTaskDelete(thread->handle);
#endif

  // Free memory
  free(thread);
  return TT_SUCCESS;
}

#endif /* TT_CAP_THREADS */
