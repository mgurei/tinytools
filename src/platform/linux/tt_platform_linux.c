/**
 * @file tt_platform_linux.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-31
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */
#define _POSIX_C_SOURCE 199309L

#include "../internal/tt_platform_internal.h"
#include "tt_mutex.h"
#include "tt_platform.h"
#include "tt_thread.h"
#include "tt_thread_internal.h"
#include "tt_types.h"
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

static struct sysinfo si;

/**
 * @brief Internal definition of Linux-specific mutex data
 */
typedef struct {
  pthread_mutex_t pthread_mutex;
} tt_linux_mutex_t;

tt_error_t tt_platform_linux_init(tt_platform_info_t *info) {
  if (info == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  // Get system information
  if (sysinfo(&si) != 0) {
    return TT_ERROR_PLATFORM_INIT;
  }

  info->platform_type = TT_PLATFORM_LINUX;

// Detect architecture
#if defined(__x86_64__)
  info->arch_type = TT_ARCH_X64;
#elif defined(__i386__)
  info->arch_type = TT_ARCH_X86;
#elif defined(__arm__)
  info->arch_type = TT_ARCH_ARM;
#elif defined(__aarch64__)
  info->arch_type = TT_ARCH_ARM64;
#else
  info->arch_type = TT_ARCH_UNKNOWN;
#endif

  // set capabilities
  info->capabilities = TT_PLATFORM_CAP_THREADS | TT_PLATFORM_CAP_MUTEX |
                       TT_PLATFORM_CAP_SEMAPHORE | TT_PLATFORM_CAP_TLS |
                       TT_PLATFORM_CAP_ATOMIC | TT_PLATFORM_CAP_TIMERS;

  info->system.cpu_frequency = 0; // TODO: implement CPU freq det
  info->system.ram_size = si.totalram;
  info->system.flash_size = 0; // Not applicable to Linux
  info->system.core_count = sysconf(_SC_NPROCESSORS_ONLN);

  // Thread capabilities
  info->thread.min_stack_size = _SC_THREAD_STACK_MIN;
  info->thread.max_stack_size = 8 * 1024 * 1024; // 8MB default
  info->thread.min_priority = sched_get_priority_min(SCHED_OTHER);
  info->thread.max_priority = sched_get_priority_max(SCHED_OTHER);

  // IRQ capabilities
  info->irq.irq_levels = 0;
  info->irq.nested_irq = false;

  return TT_SUCCESS;
}

tt_error_t tt_platform_linux_cleanup() { return TT_SUCCESS; }

#if defined(TT_CAP_THREADS)
tt_error_t tt_platform_thread_create(tt_thread_t *thread,
                                     const tt_thread_attr_t *attr,
                                     tt_thread_func_t func, void *arg) {
  if (thread == NULL || func == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  pthread_attr_t pthread_attr;
  int ret = pthread_attr_init(&pthread_attr);
  if (ret != 0) {
    return TT_ERROR_THREAD_CREATE;
  }

  if (attr != NULL) {
    if (attr->stack_size > 0) {
      ret = pthread_attr_setstacksize(&pthread_attr, attr->stack_size);
      if (ret != 0) {
        pthread_attr_destroy(&pthread_attr);
        return TT_ERROR_THREAD_CREATE;
      }
    }
  }

  ret = pthread_create(&thread->handle, &pthread_attr, func, arg);
  pthread_attr_destroy(&pthread_attr);

  return (ret == 0) ? TT_SUCCESS : TT_ERROR_THREAD_CREATE;
}

tt_error_t tt_platform_thread_join(tt_thread_t *thread, void **retval) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }
  return (pthread_join(thread->handle, retval) == 0) ? TT_SUCCESS
                                                     : TT_ERROR_THREAD_JOIN;
}

/* tt_error_t tt_thread_get_state(const tt_thread_t *thread, */
/*                                tt_thread_state_t *state); */

tt_error_t tt_platform_thread_set_priority(tt_thread_t *thread,
                                           tt_thread_priority_t priority) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }
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
}

tt_error_t tt_platform_thread_suspend(tt_thread_t *thread) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }
  // Linux doesn't have direct thread suspension
  // TODO: See what to do about this
  return TT_ERROR_NOT_IMPLEMENTED;
}

tt_error_t tt_platform_thread_resume(tt_thread_t *thread) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  // Linux doesn't have direct thread suspension
  // TODO: See what to do about this
  return TT_ERROR_NOT_IMPLEMENTED;
}

tt_error_t tt_platform_thread_sleep(uint32_t ms) {
  struct timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * 1000000;
  return (nanosleep(&ts, NULL) == 0) ? TT_SUCCESS : TT_ERROR_THREAD_SLEEP;
}

tt_thread_t *tt_platform_thread_self(void) {
  pthread_t handle = pthread_self();
  return tt_thread_table_find_by_handle(handle);
}

tt_error_t tt_platform_thread_destroy(tt_thread_t *thread) {
  if (thread == NULL) {
    return TT_ERROR_NULL_POINTER;
  }
  return (pthread_detach(thread->handle) == 0) ? TT_SUCCESS
                                               : TT_ERROR_THREAD_DETACH;
}

// TODO: See if to implement it in tt_mutex.h
tt_error_t tt_platform_thread_exit(void *retval) {
  pthread_exit(retval);
  return TT_SUCCESS;
}

#endif /* TT_CAP_THREADS */

#ifdef TT_CAP_MUTEX
/* tt_error_t tt_platform_mutex_init(tt_mutex_t *mutex, */
/*                                   const tt_mutex_attr_t *attr) { */
tt_error_t tt_platform_mutex_init(tt_mutex_t *mutex) {
  if (mutex == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  tt_linux_mutex_t *linux_mutex =
      (tt_linux_mutex_t *)malloc(sizeof(tt_linux_mutex_t));
  if (linux_mutex == NULL) {
    return TT_ERROR_MEMORY;
  }

  int res = pthread_mutex_init(&linux_mutex->pthread_mutex, NULL);
  if (res != 0) {
    free(linux_mutex);
    return TT_ERROR_MUTEX_INIT;
  }

  mutex->lock = linux_mutex;
  mutex->initialized = true;

  return TT_SUCCESS;
}

tt_error_t tt_platform_mutex_destroy(tt_mutex_t *mutex) {
  if (mutex == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  tt_linux_mutex_t *linux_mutex = (tt_linux_mutex_t *)mutex->lock;
  int res = pthread_mutex_destroy(&linux_mutex->pthread_mutex);
  if (res != 0) {
    return TT_ERROR_MUTEX_DESTROY;
  }

  free(linux_mutex);
  mutex->initialized = false;

  return TT_SUCCESS;
}

tt_error_t tt_platform_mutex_lock(tt_mutex_t *mutex) {
  if (mutex == NULL) {
    return TT_ERROR_NULL_POINTER;
  }
  if (!mutex->initialized) {
    return TT_ERROR_INVALID_PARAM;
  }

  tt_linux_mutex_t *linux_mutex = (tt_linux_mutex_t *)mutex->lock;
  return (pthread_mutex_lock(&linux_mutex->pthread_mutex) == 0)
             ? TT_SUCCESS
             : TT_ERROR_MUTEX_LOCK;
}

tt_error_t tt_platform_mutex_unlock(tt_mutex_t *mutex) {
  if (mutex == NULL) {
    return TT_ERROR_NULL_POINTER;
  }
  if (!mutex->initialized) {
    return TT_ERROR_INVALID_PARAM;
  }

  tt_linux_mutex_t *linux_mutex = (tt_linux_mutex_t *)mutex->lock;
  return (pthread_mutex_unlock(&linux_mutex->pthread_mutex) == 0)
             ? TT_SUCCESS
             : TT_ERROR_MUTEX_UNLOCK;
}

tt_error_t tt_platform_mutex_trylock(tt_mutex_t *mutex) {
  if (mutex == NULL) {
    return TT_ERROR_NULL_POINTER;
  }
  if (!mutex->initialized) {
    return TT_ERROR_INVALID_PARAM;
  }

  tt_linux_mutex_t *linux_mutex = (tt_linux_mutex_t *)mutex->lock;
  return (pthread_mutex_trylock(&linux_mutex->pthread_mutex) == 0)
             ? TT_SUCCESS
             : TT_ERROR_BUSY;
}
bool tt_platform_mutex_is_locked(tt_mutex_t *mutex) {
  if (mutex == NULL || !mutex->initialized) {
    return false;
  }

  tt_linux_mutex_t *linux_mutex = (tt_linux_mutex_t *)mutex->lock;
  if (pthread_mutex_trylock(&linux_mutex->pthread_mutex) == 0) {
    pthread_mutex_unlock(&linux_mutex->pthread_mutex);
    return false;
  }
  return true;
}

#endif /* TT_CAP_MUTEX */

tt_error_t tt_platform_mem_init(void) {
  // Not much to do in Linux userspace
  return TT_SUCCESS;
}

size_t tt_platform_mem_get_free(void) {
  if (sysinfo(&si) != 0) {
    return 0;
  }
  return si.freeram;
}
