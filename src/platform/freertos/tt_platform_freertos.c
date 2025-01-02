/**
 * @file tt_platform_freertos.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-31
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "../internal/tt_platform_internal.h"

// FreeRTOS specific defines - these would typically come from FreeRTOSConfig.h
#ifndef configCPU_CLOCK_HZ
#define configCPU_CLOCK_HZ 100000000UL // Default to 100MHz if not defined
#endif

#ifndef configMINIMAL_STACK_SIZE
#define configMINIMAL_STACK_SIZE 128 // Default minimal stack size
#endif

#ifndef configTOTAL_HEAP_SIZE
#define configTOTAL_HEAP_SIZE (48 * 1024) // Default heap size: 48KB
#endif

#ifndef configMAX_PRIORITIES
#define configMAX_PRIORITIES 5 // Default max priority levels
#endif

tt_error_t tt_platform_freertos_init(tt_platform_info_t *info) {
  if (info == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  // Basic freeRTOS platform info
  info->platform_type = TT_PLATFORM_FREERTOS;
  info->arch_type = TT_ARCH_UNKNOWN; // TODO: Should be base on target
  info->capabilities = TT_PLATFORM_CAP_THREADS | TT_PLATFORM_CAP_MUTEX |
                       TT_PLATFORM_CAP_SEMAPHORE | TT_PLATFORM_CAP_TIMERS;

  // System information
  info->system.cpu_frequency = configCPU_CLOCK_HZ;
  info->system.ram_size = 0;   // TODO: implement specific
  info->system.flash_size = 0; // TODO: implment specific
  info->system.core_count = 1;

  // Thread capabilities
  info->thread.min_stack_size = configMINIMAL_STACK_SIZE;
  info->thread.max_stack_size = configTOTAL_HEAP_SIZE;
  info->thread.min_priority = 0;
  info->thread.max_priority = configMAX_PRIORITIES - 1;

  return TT_SUCCESS;
}

tt_error_t tt_platform_freertos_cleanup(void) { return TT_SUCCESS; }
